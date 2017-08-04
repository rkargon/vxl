#ifndef bstm_multi_block_metadata_h_
#define bstm_multi_block_metadata_h_
//:
// \file bstm_multi_block_metadata.h
// \brief  bstm_multi_block_metadata data keeps track of non structure, non
// visual data for bstm_multi_block. In addition, this also keeps track of the
// number and order of space/time subdivisions in the block.
//
// \author Raphael Kargon
// \date   31 Jul 2017
//

#include <vbl/vbl_ref_count.h>
#include <vcl_compiler.h>
#include <vcl_string.h>
#include <vcl_utility.h>
#include <vcl_vector.h>
#include <vgl/vgl_box_3d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_vector_3d.h>
#include <vsl/vsl_basic_xml_element.h>

#include <boxm2/boxm2_block_metadata.h>
#include <bstm/basic/bstm_block_id.h>

//: Used to distinguish between time and space subdivisions in a block.
enum space_time_enum { STE_SPACE, STE_TIME };

//: Returns "space" or "time"
vcl_string ste_as_string(space_time_enum ste);

//: Converts a string that is either "space" or "time" to a corresponding enum
// value.
// \param s      the string to convert
// \param ste    stores resulting value
// \returns      whether or not string is valid
bool ste_from_string(const vcl_string &s, space_time_enum &ste);

//: Accepts a list of subdivisions separated by commas, e.g.
// 'space,time,space,time,space' and returns a vector of corresponding enum
// values.
vcl_vector<space_time_enum> parse_subdivisions(const vcl_string &s);

//: Prints subdivisions as a comma-separated list of space_time_enum
// values (i.e. either 'space' or 'time'.)
vcl_string print_subdivisions(const vcl_vector<space_time_enum> &subdivisions);

class bstm_multi_block_metadata : public vbl_ref_count {
public:
  bstm_multi_block_metadata() {}
  bstm_multi_block_metadata(const bstm_block_id& id,
                            const vgl_box_3d<double>& bbox,
                            vcl_pair<double, double> bbox_t,
                            double max_mb,
                            double p_init,
                            const vcl_vector<space_time_enum> &subdivisions,
                            int version = 1)
      : id_(id)
      , bbox_(bbox)
      , bbox_t_(bbox_t)
      , max_mb_(max_mb)
      , p_init_(p_init)
      , version_(version)
      , subdivisions_(subdivisions) {}

  bstm_multi_block_metadata(const bstm_multi_block_metadata &that)
      : vbl_ref_count()
      , id_(that.id_)
      , bbox_(that.bbox_)
      , bbox_t_(that.bbox_t_)
      , max_mb_(that.max_mb_)
      , p_init_(that.p_init_)
      , version_(that.version_)
      , subdivisions_(that.subdivisions_) {}

  //: id and local origin of the block
  bstm_block_id id_;
  vgl_box_3d<double> bbox_;

  //: time bounds
  vcl_pair<double, double> bbox_t_;

  // TODO: Should we allow a flat array of sub-blocks at the highest level, or
  // require one root tree for every block?
  // //: dimension of time axis, e.g., 1s
  // double   sub_block_dim_t_;

  // //: number of sub-blocks time dimension
  // unsigned   sub_block_num_t_;

  // //: World dimensions of a sub block .e.g 1 meter x 1 meter x 1 meter
  // vgl_vector_3d<double>   sub_block_dim_;

  // //: number of sub blocks in each dimension
  // vgl_vector_3d<unsigned> sub_block_num_;

  double max_mb_; // each total block mb
  double p_init_; // initialize occupancy probs with this
  int version_;

  //: Describes order of hierarchical subdivisions of block: e.g. time first,
  // then space, etc.
  vcl_vector<space_time_enum> subdivisions_;

  //: checks if this block contains the given t, if yes, returns local_time
  bool contains_t(double const t, double &local_time) const;

  //: bounding box for this block
  vgl_box_3d<double> bbox() const { return bbox_; }
  void bbox_t(double &min_t, double &max_t) const {
    min_t = bbox_t_.first;
    max_t = bbox_t_.second;
  }
  vcl_pair<double, double> bbox_t() const { return bbox_t_; }

  bool operator==(bstm_multi_block_metadata const &m) const;
  bool operator==(boxm2_block_metadata const &m) const;

  //: Writes this block's metadata to an XML element which can later
  // be written to a file, e.g. as part of a scene.
  void to_xml(vsl_basic_xml_element &block) const;

  //: Load a block from XML attributes.
  // \param atts - XML attributes for this block. Given as an array of char*'s,
  // where attribute name and values alternate in the array.
  static bstm_multi_block_metadata from_xml(const char **atts);
};

//: Smart_Pointer typedef for bstm_block
typedef vbl_smart_ptr<bstm_multi_block_metadata> bstm_multi_block_metadata_sptr;

vcl_ostream &operator<<(vcl_ostream &s, bstm_multi_block_metadata &metadata);

//: Binary write boxm_update_bit_scene_manager scene to stream
void vsl_b_write(vsl_b_ostream &os, bstm_multi_block_metadata const &scene);
void vsl_b_write(vsl_b_ostream &os, const bstm_multi_block_metadata *&p);
void vsl_b_write(vsl_b_ostream &os, bstm_multi_block_metadata_sptr &sptr);
void vsl_b_write(vsl_b_ostream &os, bstm_multi_block_metadata_sptr const &sptr);

//: Binary load boxm_update_bit_scene_manager scene from stream.
void vsl_b_read(vsl_b_istream &is, bstm_multi_block_metadata &scene);
void vsl_b_read(vsl_b_istream &is, bstm_multi_block_metadata *p);
void vsl_b_read(vsl_b_istream &is, bstm_multi_block_metadata_sptr &sptr);
void vsl_b_read(vsl_b_istream &is, bstm_multi_block_metadata_sptr const &sptr);

#endif
