#include <bvpl/bvpl_octree/bvpl_octree_sample.h>
#include <boct/boct_tree.txx>
#include <boct/boct_tree_cell.txx>

BOCT_TREE_INSTANTIATE(short, bvpl_octree_sample<float>);
BOCT_TREE_CELL_INSTANTIATE(short, bvpl_octree_sample<float>);