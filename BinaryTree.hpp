//
// Created by Hans Dulimarta.
//

#ifndef BINARYTREES_BINARYSEARCHTREE_H
#define BINARYTREES_BINARYSEARCHTREE_H
#include <memory>
#include <iostream>
#include <stdexcept>
#include <queue>
#include <stack>

using namespace std;

namespace gv {
    template<typename E>     // textbook code E is Comparable
    class BinaryTree {

    public:
        /** WARNING: You will find a few overloaded functions declared in
         * both the public and private sections. This design is intentional
         * because many tree algorithms are recursive and the private counterpart
         * are the ones implementing the recursive work and they usually
         * require additional input argument(s).
         *
         * The non-recursive public functions simply call their recursive
         * counterpart to initiate the recursive work.
         */

        /**
         * Make the tree into an empty tree
         */
        void makeEmpty() noexcept {
            root.reset(nullptr);

        }

        /**
         * Insert a new data into the BST while maintaining no duplicate entries
         * @param item
         * @return true if the item can be inserted (no duplicate), false if the tree
         * already contains the same data item.
         */
        bool insert(const E &item) noexcept {
            unique_ptr<Node> node = make_unique<Node>();
            node->data = item;

            if(root == nullptr){ //if list is empty, start at root
                root = move(node);
                return true;
            }
            else {
                return insert(node, root); //otherwise figure out where it goes
            }
        }

        /**
         * Remove an item from the tree
         * @param item data to remove
         * @return true is the data is removed, false if the data is not in the tree
         * @throw length_error on an attempt to remove from an empty tree
         */
        bool remove(const E &item) {
            if(root == nullptr){
                throw length_error("Attempt to remove from empty tree");
            }
            //does item exist?
            else if(contains(item)){
                remove(item, root);
            }

            else return false; //nothing to remove
        }

        /**
         * Print the tree using in-order traversal. Separate data item by a single space
         * @param out destination of the print out
         */
        void printTree(ostream &targetStream = cout) const noexcept {
            printTree(targetStream, root);
        }

        /**
         * Find the smallest value in the tree
         * @return the smallest value
         * @throw length_error if the tree is empty
         */
        const E findMin() const {
            if(root == nullptr){
                throw length_error("Tree is empty");
            }
            else{
                Node* current = root.get(); //the data from root is put into a temp raw pointer
                while (current->left.get() != nullptr){
                    current = current->left.get();
                }
                return current->data;
            }
        }

        /**
         * Find the largest value in the tree
         * @return the largest value
         * @throw length_error if the tree is empty
         */
        const E findMax() const {
            if(root == nullptr){
                throw length_error("Tree is empty");
            }
            else{
                Node* current = root.get(); //the data from root is put into a temp raw pointer
                while (current->right.get() != nullptr){
                    current = current->right.get();
                }
                return current->data;
            }
        }

        /**
         * Check if the given item is stored in the tree
         * @param val
         * @return true if the item is stored in the tree, false otherwise
         */
        bool contains(const E &val) const noexcept {
                return contains(val, root);
        }

        /**
         * Is the tree empty?
         * @return
         */
        bool isEmpty() const noexcept {
            return root == nullptr;
        }

        /**
         * Return the number of nodes in the tree (Problem 4.31a)
         * @return
         */
        int numberOfNodes() const noexcept {
            if(root != nullptr) {
                return 1 + numberOfNodes(root);
            }
            else{
                return 0;
            }
        }

        /**
         * Return the number of leaves in the tree (Problem 4.31b)
         * @return
         */
        int numberOfLeaves() const noexcept {
            return numberOfLeaves(root);
        }

        /**
         * Return the number of full nodes (Problem 4.31c). A full node is a node with exactly two children
         * @return
         */
        int numberOfFullNodes() const noexcept {
            return numberOfFullNodes(root);
        }

        /**
         * Remove all the leaves from the tree and keep the data in these leaves into a vector
         * @return a vector of removed items
         */
        vector<E> remove_leaves() noexcept {
            vector<E> prunedLeaves;
            unique_ptr<Node> nullpointer = make_unique<Node>();
            remove_leaves(nullpointer, root, prunedLeaves);
            return prunedLeaves;
        }

        /**
         * Visit the node in level order (Problem 4.40)
         * @return
         */
        vector<E> levelOrder() const {
            vector<E> out;
            for(int i = 1; i <= getHeight(root) + 1; i++){
                levelOrder(root, i, out);
            }
            return out;
        }

        static bool hasLeak() {
            return nodeCount != 0;
        }

        static int allocatedNodes() {
            return nodeCount;
        }

    private:
        struct Node;

        //----recursive helpers----
        bool insert(unique_ptr<Node>& newNode, unique_ptr<Node>& current) {
            if (newNode->data == current->data){ //duplicate
                return false;
            }
            else if (current->data > newNode->data){
                //for threaded BST, we insert if left is null, or if leftThread is false
                if(current -> left == nullptr ){
                    current->left = move(newNode);
                    return true;
                }
                else{
                    //great! we found the place we want to insert the node!
                    insert(newNode, current->left);

                    //now we need to make previous ->leftThread true!

                    //we also need to figure out where to point our threads.
                }
            }
            else if (current->data < newNode->data){
                if(current->right == nullptr){
                    current->right = move(newNode);
                    return true;
                }
                else{
                    //great! we found the place we want to insert the node!
                    insert(newNode, current->right);
                }
            }
        }
        bool remove( const E & data, unique_ptr<Node>& current ) {
            //Return false if no item is found
            if( current == nullptr )
                return false;
            //data not found, recurse until found
            if( data < current->data )
                return remove( data, current->left );
            else if( current->data < data )
                return remove( data, current->right);

            //data found
            else if(current->data == data
                    && current->left != nullptr
                    && current->right != nullptr ) // Two children
            {
                //find the min of the right subtree and store it
                Node *tmp = current->right.get();
                E min;
                while(tmp->left != nullptr) {
                    tmp = tmp->left.get();
                }
                min = tmp->data;

                //replace the data of the node to delete
                current->data = min;

                //remove the min data node since it was "moved"
                return remove( current->data, current->right );
            }

            else if (current->data == data
                     && (current->left == nullptr || current->right == nullptr)) //one child
            {
                //make a node
                unique_ptr<Node> oldNode = make_unique<Node>();

                //set that node equal to the node to remove
                oldNode = move(current);

                //set the node to remove to either its left or right child
                current = ( oldNode->left != nullptr ) ? move(oldNode->left) : move(oldNode->right);

                //delete the unneeded node
                oldNode.reset();

                //delete oldNode;
            }
            else //no children, just get rid of it
                current.reset();
            //delete current;
            return true;
        }
        void printTree(ostream &printer, const unique_ptr<Node> &current) const{
            if(current->left != nullptr){ //print left first
                printTree(printer, current->left);
            }
            printer << current->data << " ";
            if(current->right != nullptr){
                printTree(printer, current->right);
            }

        }
        bool contains(const E& val, const unique_ptr<Node> &current) const {
            if(current == nullptr) //if is null
                return false;
            else if(val > current->data) //if val is in the right subtree
                return contains(val, current->right);
            else if(val < current->data) //if val is in the left subtree
                return contains(val, current->left);

            return true; //otherwise we found it
        }
        int numberOfLeaves(const unique_ptr<Node> &current) const {
            if(current == nullptr){
                return 0;
            }
            if(isLeaf(current)){ //it's a leaf
                return 1 + numberOfLeaves(current->left) + numberOfLeaves(current->right);
            }
            else{
                return numberOfLeaves(current->left) + numberOfLeaves(current->right);
            }
        }
        int numberOfNodes(const unique_ptr<Node> &current) const {
            if(isFull(current)){ //both exist, count+2
                return 2 + numberOfNodes(current->left) + numberOfNodes(current->right);
            }
            else if(current->left != nullptr){ //only left, count+1
                return 1 + numberOfNodes(current->left);
            }
            else if(current->right != nullptr){ //only right, count+1
                return 1 + numberOfNodes(current->right);
            }
            else return 0;
        }
        int numberOfFullNodes(const unique_ptr<Node> &current) const{
            if(current == nullptr){
                return 0;
            }
            if(isFull(current)){ //its full
                return 1 + numberOfFullNodes(current->left) + numberOfFullNodes(current->right);
            }
            else{ //current is not full, check children
                return numberOfFullNodes(current->left) + numberOfFullNodes(current->right);
            }
        }
        int remove_leaves(unique_ptr<Node> &previous, unique_ptr<Node> &current, vector<E> &prunedLeaves){
            if(current == nullptr){
                return 0;
            }
            else if(isLeaf(current)){ //this is leaf
                prunedLeaves.push_back(current->data); //add to vector
                if(current == previous->left){
                    previous->left.reset();
                }
                else previous->right.reset();
                current.reset();
            }
            else{
                return remove_leaves(current, current->left, prunedLeaves) + remove_leaves(current, current->right, prunedLeaves);
            }

        }
        void levelOrder(const unique_ptr<Node> &current, int level, vector<E> &out) const{
            if(current == nullptr){
                return;
            }
            else if(level == 1){
                out.push_back(current->data);
            }
            else{
                levelOrder(current->left, level - 1, out);
                levelOrder(current->right, level - 1, out);
            }
        }

        //----helpers----
        bool isLeaf(const unique_ptr<Node> &current) const{
            if(current->left == nullptr && current->right == nullptr){
                return true;
            }
            else return false;
        }
        bool isFull(const unique_ptr<Node> &current) const{
            if(current->left != nullptr && current->right != nullptr){
                return true;
            }
            else return false;
        }
        int getHeight(const unique_ptr<Node>& current) const {
            if(current == nullptr)
                return -1;

            int left = getHeight(current->left);
            int right = getHeight(current->right);

            if (left > right)
                return 1 + left;
            else
                return 1 + right;
        }

        static int nodeCount;
        struct Node {
            Node() {
                nodeCount++;
            }
            ~Node() {
                nodeCount--;
            }
            E data;
            unique_ptr<Node> left, right;
            bool leftThread = false;
            bool rightThread = false;
        };
        unique_ptr<Node> root;
    };

    template<typename E>
    int BinaryTree<E>::nodeCount = 0;
}
#endif //BINARYTREES_BINARYSEARCHTREE_H
