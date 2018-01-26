/**
 * @file quadtree.cpp
 * Quadtree class implementation.
 */

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

#include "quadtree.h"
#include "png.h"

// Quadtree
//   - parameters: none
//   - constructor for the Quadtree class; makes an empty tree
Quadtree::Quadtree() 
{
	root = NULL;
	res = 0;
}

// Quadtree
//   - parameters: PNG const & source - reference to a const PNG
//                    object, from which the Quadtree will be built
//                 int resolution - resolution of the portion of source
//                    from which this tree will be built
//   - constructor for the Quadtree class; creates a Quadtree representing
//        the resolution by resolution block in the upper-left corner of
//        source
Quadtree::Quadtree(PNG const& source, int setresolution)
{
	root = NULL;
	buildTree(source, setresolution);
}

// Quadtree
//   - parameters: Quadtree const & other - reference to a const Quadtree
//                    object, which the current Quadtree will be a copy of
//   - copy constructor for the Quadtree class
Quadtree::Quadtree(Quadtree const& other) 
{
	if(other.root == NULL){
        root = NULL;
        return;
    }
    else {
        root = copy(other.root);
        res = other.res;
    }
}

// ~Quadtree
//   - parameters: none
//   - destructor for the Quadtree class
Quadtree::~Quadtree()
{
	 clear(root);
	 res = 0;
}

// operator=
//   - parameters: Quadtree const & other - reference to a const Quadtree
//                    object, which the current Quadtree will be a copy of
//   - return value: a const reference to the current Quadtree
//   - assignment operator for the Quadtree class
Quadtree const& Quadtree::operator=(Quadtree const& other)
{
	//check for self-assignment
    if (root == other.root){
        return *this;
    }

    clear(root);
    root = copy(other.root);
	res = other.res;
    return *this;
}

// buildTree (public interface)
//   - parameters: PNG const & source - reference to a const PNG
//                    object, from which the Quadtree will be built
//                 int resolution - resolution of the portion of source
//                    from which this tree will be built
//   - transforms the current Quadtree into a Quadtree representing
//        the resolution by resolution block in the upper-left corner of
//        source
void Quadtree::buildTree(PNG const& source, int setresolution)
{
	// we should clear the Quadtree and build a Quadtree that has enough elements for the source
    // set the resolution of the quadtree to the given one
    // recursive call on the root quadtreenode using btHelper
    clear(root);
    res = setresolution;
	btHelper(source, 0, 0, setresolution, root);
}

// btHelper helper function for buildTree
void Quadtree::btHelper(PNG const &source, int xcoord, int ycoord, int resolution, QuadtreeNode *&subroot) {
    // Create QuadtreeNode (need to fix)
    subroot = new QuadtreeNode();

    // base case when resolution == 1
    if (resolution == 1){
        subroot->element = *source(xcoord,ycoord); 
        return;
    }

    //recursively build tree
    btHelper(source, (xcoord + resolution/2), ycoord, resolution/2, subroot->neChild);
    btHelper(source, xcoord, ycoord, resolution/2, subroot->nwChild);
    btHelper(source, xcoord, (ycoord + resolution/2), resolution/2, subroot->swChild);
    btHelper(source, (xcoord + resolution/2), (ycoord + resolution/2), resolution/2, subroot->seChild);

    //color pixels
    subroot->element.red = (subroot->neChild->element.red + subroot->nwChild->element.red + subroot->swChild->element.red + subroot->seChild->element.red)/4;
    subroot->element.green = (subroot->neChild->element.green + subroot->nwChild->element.green + subroot->swChild->element.green + subroot->seChild->element.green)/4;
    subroot->element.blue = (subroot->neChild->element.blue + subroot->nwChild->element.blue + subroot->swChild->element.blue + subroot->seChild->element.blue)/4;
	subroot->element.alpha = (subroot->neChild->element.alpha + subroot->nwChild->element.alpha + subroot->swChild->element.alpha + subroot->swChild->element.alpha)/4;
}

// getPixel (public interface)
//   - parameters: int x, int y - coordinates of the pixel to be retrieved
//   - return value: an RGBAPixel representing the desired pixel of the
//        underlying bitmap
//   - retrieves and returns the pixel at coordinates (x, y) in the
//        underlying bitmap
RGBAPixel Quadtree::getPixel(int x, int y) const
{
	// need the default pixel for the out-of-bounds cases
	RGBAPixel pixel;  

	// if the picture doesn't exist, return the default pixel
	if( res == 0 )
		return pixel;
	// if the coordinates are outside of the picture, return the default pixel
	else if( (x >= res) || (y >= res) )
		return pixel;
	// if the coordinates are vaild, return the pixel, or its deepest ancestor
	else {
		QuadtreeNode* leafNode = findNode(root, x, y, res-1);
		return leafNode->element;
	}
}

// decompress (public interface)
//   - parameters: none
//   - return value: a PNG object representing this quadtree's underlying
//        bitmap
//   - constructs and returns this quadtree's underlying bitmap
PNG Quadtree::decompress() const
{
	// create a new PNG object using the default constructor
	PNG image;
	// if the quadtree is empty, return the default PNG
	if(res == 0)
		return image;

	// if the quadtree isn't empty, resize image to res
	image.resize( (size_t)res, (size_t)res );

	// for each pixel of image get the pixel from the quadtree
	for(int x = 0; x < res; x++) {
		for(int y = 0; y < res; y++) {
			*image(x,y) = getPixel(x,y);
		}
	}

	return image;
}

// clockwiseRotate (public interface)
//   - parameters: none
//   - transforms this quadtree into a quadtree representing the same
//        bitmap, rotated 90 degrees clockwise
void Quadtree::clockwiseRotate()
{
	clockwiseShift(root);
}

// prune (public interface)
//   - parameters: int tolerance - an integer representing the maximum
//                    "distance" which we will permit between a node's color
//                    (i.e. the average of its descendant leaves' colors)
//                    and the color of each of that node's descendant leaves
//   - for each node in the quadtree, if the "distance" between the average
//        of that node's descendant leaves' colors and the color of each of
//        that node's descendant leaves is at most tolerance, this function
//        deletes the subtrees beneath that node; we will let the node's
//        color "stand in for" the colors of all (deleted) leaves beneath it
void Quadtree::prune(int tolerance)
{
	// recPrune and it's helper functions do all the work for prune
	recPrune(root, tolerance);
	return;
}

// pruneSize (public interface)
//   - parameters: int tolerance - an integer representing the maximum
//                    "distance" which we will permit between a node's color
//                    (i.e. the average of its descendant leaves' colors)
//                    and the color of each of that node's descendant leaves
//   - returns the number of leaves which this quadtree would contain if it
//        was pruned using the given tolerance; does not actually modify the
//        tree
int Quadtree::pruneSize(int tolerance) const
{
	return recPruneSize(root, tolerance);
}

// idealPrune (public interface)
//   - parameters: int numLeaves - an integer representing the number of
//                    leaves we wish the quadtree to have, after pruning
//   - returns the minimum tolerance such that pruning with that tolerance
//        would yield a tree with at most numLeaves leaves
int Quadtree::idealPrune(int numLeaves) const
{
	int min = 0;
    int max = 255*255*3; // maximum differences: (255-0)^2 + (255-0)^2 + (255-0)^2
    return idealPrune(min, max, numLeaves);
}

// Helper function for idealPrune using the Binary Search
//   - parameters: int min  - minimum difference
//   - parameters: int max  - maximum difference
//   - parameters: int numLeaves - the number of leaves you want to remain in the tree
//   - return the minimum tolerance that would yield a tree with at most numLeaves leaves
int Quadtree::idealPrune(int min, int max, int numLeaves) const
{
    int leaves;
    // base case, If it can't find the numLeaves then just return the minimum tolerance we have
    if (min > max){
        return min;
    }
    // start from the middle
    leaves = pruneSize( (min+max)/2 );
    // if the midpoint is exactly equal to the numLeaves we wish for, we should do a double check
    // with the element right before midpoint just in case there is a smaller value that we want
    if (leaves == numLeaves){
        if(numLeaves == pruneSize(((min+max)/2) - 1)){
            return idealPrune(0, (((min+max)/2) - 1), numLeaves);
        }
        // if not equal just return midpoint as the minimum tolerance
        return (min+max)/2;

    }
    // if the leaves is less than the required numLeaves, which means our tolerance is too high
    // we just reassign our max to be one element before midpoint
    if (leaves < numLeaves){
        return idealPrune(min, ((min+max)/2)-1, numLeaves);
    }
    // if the leaves is greater than the required numLeaves, which means our tolerance is too low,
    // we will reassign our min value to be one element after the midpoint
    else return idealPrune(((min+max)/2)+1, max, numLeaves);

}

// QuadtreeNode
//   - parameters: none
//   - constructor for the QuadtreeNode class; creates an empty
//        QuadtreeNode, with all child pointers NULL
Quadtree::QuadtreeNode::QuadtreeNode()
{
    neChild = seChild = nwChild = swChild = NULL;
}

// QuadtreeNode
//   - parameters: RGBAPixel const & elem - reference to a const
//        RGBAPixel which we want to store in this node
//   - constructor for the QuadtreeNode class; creates a QuadtreeNode
//        with element elem and all child pointers NULL
Quadtree::QuadtreeNode::QuadtreeNode(RGBAPixel const& elem)
{
    element = elem;
    neChild = seChild = nwChild = swChild = NULL;
}

//////////////////////
// helper functions //
//////////////////////

// priveate helper method for finding a node at a given index or finding its 
// deepest ancestor
//   - parameters: int x, int y - coordinates of the pixel(node) to be retrieved
//                 int middle - pass in the size of the square to check, middle is
//                              calculated inside
//                 QuadtreeNode* node - node of the tree. Pass in the root
//   - return:     QuadtreeNode* - return a pointer to the node at the end of the
//                                 traversal
Quadtree::QuadtreeNode* Quadtree::findNode(QuadtreeNode* node, int x, int y, int middle) const
{
	while( (node->nwChild != NULL) || (node->neChild != NULL) || (node->swChild != NULL) || (node->seChild != NULL) ) {
		// calculate the node which is right before the middle of the coordinate.
		// For example, take a square image of 16x16 pixels. The value of middle passed
		// in when called from getPixel would be 15 (res-1). The next middle value would be
		// 7, after that 3, then 1, then 0.
		middle = middle / 2;

		if( y > middle ) {        
		// in this case the child node would be in the Southeastern region
			if( x > middle ) {   
				// if the southeastern child is null, return the current node as it is 
				// the deepest ancestor
				if( node->seChild == NULL )
					return node;
				else {
					// if the child is valid, node becomes it. x and y are adjusted accordingly
					// and then the function is recursively called
					node = node->seChild;
					x -= middle+1;
					y -= middle+1;
				}
			} 
			// in this case the child node would be in the Southwestern region
			else {              
				if( node->swChild == NULL )
					return node;
				else {
					node = node->swChild;
					y -= middle+1;
				}
			}
		} 
		// in either of these cases the child node would be in the Northern region 
		else {             
			// Northeastern region
			if( x > middle ) {  
				if( node->neChild == NULL )
					return node;
				else {
					node = node->neChild;
					x -= middle+1;
				}
			}	 
			// Northwestern region
			else {               
				if( node->nwChild == NULL )
					return node;
				else {
					node = node->nwChild;
				}
			}
		}
	}
	return node;
}

// This is a recursive helper method for clockwiseRotate().
// It does the actual rotations.
//   - parameters: QuadtreeNode * &node - the root of a tree
//   - return:     nothing
void Quadtree::clockwiseShift(QuadtreeNode* &node)
{
	// if the node has no children theres nothing to shift/rotate
	if( (node->nwChild == NULL) && (node->neChild == NULL) &&
		(node->swChild == NULL) && (node->seChild == NULL) ) {
			return;
	}

	// shift all the nodes over by one position clockwise
	// For example: A  B -> C  A
	//              C  D    D  B
	QuadtreeNode* temp = node->swChild;
	node->swChild = node->seChild;
	node->seChild = node->neChild;
	node->neChild = node->nwChild;
	node->nwChild = temp;

	// call clockwiseShift on the children of the current node
	clockwiseShift(node->swChild);
	clockwiseShift(node->seChild);
	clockwiseShift(node->neChild);
	clockwiseShift(node->nwChild);
}

// helper function for Quadtree copy constructor
// takes a copyRoot if it is NULL return NULL
// If not, create the new QuadtreeNode using its element and copy every child
// then return the copy
Quadtree::QuadtreeNode *Quadtree::copy(QuadtreeNode *cRoot) {
    if(cRoot == NULL){
        return NULL;
    }
    QuadtreeNode *out = new Quadtree::QuadtreeNode(cRoot->element);
    out->nwChild = copy(cRoot->nwChild);
    out->neChild = copy(cRoot->neChild);
    out->seChild = copy(cRoot->seChild);
    out->swChild = copy(cRoot->swChild);
    return out;
}

// helper function for Quadtree destructor
void Quadtree::clear(QuadtreeNode *& subroot){
   // base case
   if (subroot == NULL){
       return;
   }
   // clear all nodes recursively
   clear(subroot->neChild);
   clear(subroot->nwChild);
   clear(subroot->swChild);
   clear(subroot->seChild);
   delete subroot;
   subroot = NULL;
}

// Helper method for prune, which recursively prunes all the nodes it needs to
//   - parameter: QuadtreeNode* &subroot - the starting point of the pruning
//   - parameter: int tolerance - if the difference in color between a node and
//                                each of its children is less than tolerance,
//                                pruning is done
//   - returns: nothing
void Quadtree::recPrune(QuadtreeNode* &subroot, int tolerance)
{
	// If the subroot has no children, return as we have reached the end of 
	// this path down the tree
	if( (subroot->nwChild == NULL) && (subroot->neChild == NULL) && 
		(subroot->swChild == NULL) && (subroot->seChild == NULL) ) {
			return;
	}
	// vector for containing all the leaves of this subroot
	std::vector<QuadtreeNode*> leaves;
	// get the leaves
	getLeaves(subroot, leaves);
	// if all of the leaves are within the tolerance, then clear the four children
	// of this subroot
	if( checkLeaves(subroot, leaves, tolerance) ) {
		clear(subroot->nwChild);
		clear(subroot->neChild);
		clear(subroot->swChild);
		clear(subroot->seChild);
	}
	// else try to prune the subroot's childen
	else {
		recPrune(subroot->nwChild, tolerance);
		recPrune(subroot->neChild, tolerance);
		recPrune(subroot->swChild, tolerance);
		recPrune(subroot->seChild, tolerance);
	}
	return;
}

// Helper method for pruneSize, which, using recursion, returns the number of leaves
// this quadtree would have if it were pruned. This function does not actually prune
// the tree
//   - parameter: QuadtreeNode* subroot - the starting point of the pruning
//   - parameter: int tolerance - if the difference in color between a node and
//                                each of its children is less than tolerance,
//                                pruning is done
//   - returns: number of leaves a quadtree would have, after it is pruned
int Quadtree::recPruneSize(QuadtreeNode* subroot, int tolerance) const
{
	// If the subroot has no children, we have reached the end of this path 
	// down the tree. Return 1 because this is a leaf of the tree
	if( (subroot->nwChild == NULL) && (subroot->neChild == NULL) && 
		(subroot->swChild == NULL) && (subroot->seChild == NULL) ) {
			return 1;
	}
	// vector for containing all the leaves of this subroot
	std::vector<QuadtreeNode*> leaves;
	// get the leaves
	getLeaves(subroot, leaves);
	// if all the leaves of this subroot are within the tolerance, then return 1
	// because the children and their subtrees would be deleted; this subroot
	// would be pruned
	if( checkLeaves(subroot, leaves, tolerance) ) {
		return 1;
	}
	// else call redPruneSize on this subroot's children and return the sum of
	// their return values
	else {
		return recPruneSize(subroot->nwChild, tolerance) + 
			   recPruneSize(subroot->neChild, tolerance) +
	 	       recPruneSize(subroot->swChild, tolerance) +
		       recPruneSize(subroot->seChild, tolerance);
	}
}

// Helper function for recPrune that gets all the leaves of a particular subroot
//   - parameter: QuadtreeNode* subroot - all of the children of this subroot will be found
//   - parameter: vector<QuadtreeNode*> &leaves - a vector containing all of the leaves of the subroot
//   - returns: nothing
void Quadtree::getLeaves(QuadtreeNode* subroot, vector<QuadtreeNode*> &leaves) const
{
	// if subroot has no children, then it is a leaf and must be added to leaves
	if( (subroot->nwChild == NULL) && (subroot->neChild == NULL) && 
		(subroot->swChild == NULL) && (subroot->seChild == NULL) ) {
			leaves.push_back(subroot);
			return;
	}
	// else get the leaves of its children
	getLeaves(subroot->nwChild, leaves);
	getLeaves(subroot->neChild, leaves);
	getLeaves(subroot->swChild, leaves);
	getLeaves(subroot->seChild, leaves);
}

// Helper method for recPrune that checks all of the leaves of a subroot against the tolerance
// to see if the subroot should be pruned
//   - parameter: QuadtreeNode* subroot - the subroot 
//   - parameter: vector<QuadtreeNode*> leaves - the leaves of the subroot
//   - parameter: int tolerance - the tolerance of the difference
//   - return: bool - true if all the leaves are within the tolerance, false otherwise
bool Quadtree::checkLeaves(QuadtreeNode* subroot, vector<QuadtreeNode*> leaves, int tolerance) const
{
	// declare an iterator for leaves
	std::vector<QuadtreeNode*>::iterator it;

	// iterating through leaves, if the difference between any of the leaves and subroot is 
	// greater than tolerance, return false
	for(it = leaves.begin(); it != leaves.end(); it++)
		if( calcDifference(subroot, *it) > tolerance )
			return false;

	// if the iterator goes through the entire vector, then the tolerance was met for 
	// every leaf, so return true
	return true;
}

// Helper function for redPrune which calculates the difference between two RGB colors
//   - parameter: QuadtreeNode* subrot - pointer to a subroot in a Quadtree object
//   - parameter: QuadtreeNode* child - pointer to one of the four children of the subroot
//   - return: the integer value of the differnce between the two colors
int Quadtree::calcDifference(QuadtreeNode* subroot, QuadtreeNode* child) const
{
	return pow( (subroot->element.red   - child->element.red), 2 ) +
		   pow( (subroot->element.green - child->element.green), 2) +
		   pow( (subroot->element.blue  - child->element.blue), 2);
}

