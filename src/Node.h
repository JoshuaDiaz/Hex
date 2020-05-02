// Generic node class

#ifndef NODE_H
#define NODE_H
#include <vector>
#include <iostream>

using namespace std;

template <typename T>
class Node
{
    private:
    // unique id for this node
    int id;
    // list of pointers to all other nodes this node connects to (adjacency list)
    vector<Node *> edgeList;
    // data that occupies this node
    T occupier; 

    public:
    /* Constructor **/
    Node(int id, T occupier);
    /* Destructor **/
    ~Node();
    /* Return Node ID **/
    int getNodeID(void) const;
    /* Return node occupier **/
    T getOccupier(void) const;
    /* Set this node's occupier **/
    void setOccupier(T occ);
    /* Add a node to this node's adjacency list **/
    void addEdge(Node *n);
    /* Remove a node from this node's adjacency list **/
    void removeEdge(Node *n);
    /* Return the amount of nodes in this node's adjacency list **/
    int getEdgeListSize(void) const;
    /* Return a reference to this node's adjacency list **/
    vector<Node *>& getEdgeList(void);
    /* Print node to an ostream (prints its occupier) */
    template <typename U>
    friend ostream& operator<< (ostream& out, const Node<U>& n);
    /* Print node to an ostream given its pointer (prints its occupier) */
    template <typename U>
    friend ostream& operator<< (ostream& out, const Node<U>*& n);
};

#include "Node.tpp"

#endif //NODE_H