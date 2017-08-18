#ifndef bstm_multi_bstm_block_to_bstm_multi_block_function_h_
#define bstm_multi_bstm_block_to_bstm_multi_block_function_h_

#include <vcl_map.h>
#include <vcl_stack.h>
#include <vcl_string.h>

#include <bstm/bstm_block.h>
#include <bstm/bstm_block_metadata.h>
#include <bstm/bstm_data_base.h>
#include <bstm/bstm_data_traits.h>
#include <bstm/bstm_util.h>

#include <bstm_multi/basic/array_4d.h>
#include <bstm_multi/bstm_multi_block_metadata.h>
#include <bstm_multi/bstm_multi_typedefs.h>
#include <bstm_multi/space_time_scene.h>

// Returns the volume of a 4D region given as a vector of <x,y,z> coordinates
// and a time length.
// This is a just product of all the given dimension.
template <typename T> T volume(const vcl_pair<vgl_vector_3d<T>, T> &v);

// \brief Gets an appearance and an alpha model from a collection of
// data buffers. If there are multiple appearance models, only the
// first one is used. The buffers are stored in-place in 'alpha' and
// 'appearance', respectively.
//
// \param bstm_datas - map of data type prefix to data buffer
//
// \param alpha - pointer to occupancy data buffer. This value is modified by
// this function to point to the appropriate data buffer in `bstm_datas`.
//
// \param appearance - pointer to appearance data buffer. This value
// is modified by this function to point to the appropriate data
// buffer in `bstm_datas`.
//
// \param appearance_type - appearance data type as string
void get_bstm_data_buffers(
    const vcl_map<vcl_string, bstm_data_base *> &bstm_datas,
    bstm_data_base *&alpha,
    bstm_data_base *&appearance,
    vcl_string &appearance_type);

// \brief Imports space and time trees from a BSTM block into a multi-BSTM
// block.
//
// This is not a simple memcpy. BSTM's space trees are stored in row-major of
// (x,y,z) coordinates. Each space tree points to a buffer of time trees
// containing num_time_trees * num_leaves such trees. Specifically, each *leaf*
// of the space tree points to num_time_trees contiguous time trees.
//
// We would like to have (x,y,z,num_time_trees) space trees in row-major order,
// where each space tree points to num_leaves contiguous time trees. Thus we
// need to shuffle the time trees (and data buffers) around, as well as create
// num_time_trees extra copies of each space tree.
//
// \param blk - Multi-BSTM block into which space trees are copied.
//
// \param bstm_blk - BSTM block containing space trees
//
// \param bstm_blk_t - BSTM time block containing time trees
//
// \param current_level - The level (0-indexed) of the new Multi-BSTM
// block into which trees are copied. Usually, this level is
// MAX_LEVELS-1. (i.e. time and space trees are copied into the last two levels)
//
// \param alpha - BSTM's occupancy data buffer
//
// \param alpha_new - Multi-BSTM's occupancy data buffer, which is replaced with
// a new data buffer the same size as 'alpha' containing copied-over occupancy
// data.
//
// \param appearance - BSTM's appearance data buffer
//
// \param appearance_new - Multi-BSTM's appearance data buffer, which
// is replaced with a new data buffer the same size as 'appearance'
// containing copied-over appearance data.
//
// \param appearance_type - apearance data type
void convert_bstm_space_trees(bstm_multi_block *blk,
                              bstm_block *bstm_blk,
                              bstm_time_block *bstm_blk_t,
                              vcl_size_t current_level,
                              const bstm_data_base *alpha,
                              block_data_base *alpha_new,
                              const bstm_data_base *appearance,
                              block_data_base *appearance_new,
                              const vcl_string &appearance_type);

// \brief convenience function for choosing a template specialization
// (based on appearance data type) at run-time using the appearance
// type string.
//
// \returns output of time_differences_from_bstm_trees(...) if data
// type is valid. If not, returns an empty vcl_vector<bool>.
vcl_vector<bool> dispatch_time_differences_from_bstm_trees(
    unsigned char *time_buffer,
    unsigned char *space_buffer,
    const vcl_pair<vgl_vector_3d<unsigned>, unsigned> &num_regions,
    const block_data_base *alpha,
    const block_data_base *appearance,
    const vcl_string &appearance_type,
    double p_threshold,
    double app_threshold);

// \brief Given BSTM data, creates a 4D array of booleans that
// determines, for each region of space (corresponding to a space tree
// in BSTM), whether that region is the same as the corresponding
// region of space in the previous time frame.  This array is used to
// further coalesce the BSTM scene over time during the creation of
// the multi-BSTM scene. Two time frames are similar if they are a)
// totally unrefined or b) composed of one voxel who's appearance and
// occupancy data changes less than the given threshold over those two
// time frames.
//
// NOTE: By "BSTM data", it means actually output from
// convert_bstm_space_trees which is in row-major order over all 4
// dimensions, not raw BSTM data which is differently formatted.
//
// \tparam APP_TYPE - appearance data type.
//
// \param time_buffer - BSTM time trees, arranged in row major order
// over (x,y,z,t).
//
// \param space_buffer - BSTM space trees, arranged in row major order over
// (x,y,z,t). Note that each voxel of a space tree points to exactly one time
// tree. That is, instead of directly using BSTM space trees, use output from
// convert_bstm_space_trees. Essentially, if the scene is too long over time to
// store in one time tree, there should be one space tree for each 32-frame time
// region.
//
// \param num_regions - Number of space trees in each x,y,z,t dimension.
// \param alpha - Buffer of occupancy data
// \param app_threshold - Buffer of appearance data
//
// \returns A vector of booleans where each corresponds to a space
// tree, arranged in row-major order. The boolean representing region
// (x,y,z,t) is true if this region is significantly different from
// (x,y,z,t-1) and false otherwise.
template <bstm_data_type APP_TYPE>
vcl_vector<bool> time_differences_from_bstm_trees(
    unsigned char *time_buffer,
    unsigned char *space_buffer,
    const vcl_pair<vgl_vector_3d<unsigned>, unsigned> &num_regions,
    const block_data<BSTM_ALPHA> &alpha,
    const block_data<APP_TYPE> &appearance,
    double p_threshold,
    double app_threshold);

// Given a space tree and a buffer of lower-level trees, sets the
// structure bits of the given space tree to represent the low-level
// trees. i.e. if a whole bunch of the lower-level trees are
// unrefined, the coresponding inner node for the space tree will also
// be unrefined.
//
// \param current_tree - the space tree that is to contain the
// lower-level trees.
//
// \param num_regions - The number of trees in each dimension at the
// current level (i.e. x, y, z, time).
//
// \param coords - Indices corresponding to current_tree's position in
// the current level.
//
// \param child_level_buffer - Buffer containing trees at the lower
// level. These can be either all space trees or all time trees. Also,
// since a space tree represents a space of 8^=512 sub-voxels, if
// num_regions=[x,y,z,t], then the size of the child level should be
// [x/8, y/8, z/8, t]. Trees should be arranged in row-major order.
//
// \param child_level_type - Whether the lower level contains time
// trees or space trees.
//
// \param time_differences_vec - vector showing whether each region in
// space-time is different from the previous on in time. This needs to
// be coalesced over space to accurately represent the new, unrefined
// tree.
bool make_unrefined_space_tree(
    boct_bit_tree &current_tree,
    const vcl_pair<vgl_vector_3d<unsigned>, unsigned> &num_regions,
    const index_4d &coords,
    unsigned char *child_level_buffer,
    space_time_enum child_level_type,
    const vcl_vector<bool> &time_differences_vec);

// \brief Similar to make_unrefined_space_tree, except coalesces over
// time. The difference here is that instead of coalescing based on
// whether sub-regions are refined or not, this coalesces based on
// similarity over time.
//
// \param current_tree - time tree that represents the region in
// question. This is filled with the appropriate bits.
//
// \param tree_index - index of the tree in the list of all trees at
// this current level
//
// \param time_differences_vec - whether each region in space-time is
// similar to the same region in the previous frame.
void make_unrefined_time_tree(bstm_time_tree &current_tree,
                              vcl_size_t tree_index,
                              const vcl_vector<bool> &time_differences_vec);

//: Data structure for representing a node in a bstm_multi_block. This is
// returend by iterators traversing the block's data.
struct bstm_multi_block_tree_node {
  const generic_tree tree;
  index_4d coords;
  int level;
};

bool bstm_block_to_bstm_multi_block(
    bstm_multi_block *blk,
    const vcl_map<vcl_string, block_data_base *> &datas,
    bstm_block *bstm_blk,
    bstm_time_block *bstm_blk_t,
    const vcl_map<vcl_string, bstm_data_base *> &bstm_datas,
    double p_threshold,
    double app_threshold);

#endif // bstm_multi_bstm_to_bstm_multi_scene_function_h_
