// Generic Node class template implementation.
// must be included at the end of Node.h

template <typename T>
Node<T>::Node(int id, T occupier) : id(id), occupier(occupier)
{
}

template <typename T>
Node<T>::~Node()
{
}

template <typename T>
void Node<T>::addEdge(Node *n)
{
    if(n != nullptr && n->id != this->id)
        edgeList.push_back(n);
}

template <typename T>
void Node<T>::removeEdge(Node *n)
{
    if(n != nullptr){
        for(auto x = this->edgeList.begin(); x != this->edgeList.end(); x++){
            if(*x == n){
                edgeList.erase(x);
                x--; //decrement iterator since current position is ahead now.
            }
        }
    }
}

template <typename T>
vector<Node<T>* >& Node<T>::getEdgeList(void)
{
    return this->edgeList;
}

template <typename T>
int Node<T>::getEdgeListSize(void) const
{
    return this->edgeList.size();
}

template <typename T>
int Node<T>::getNodeID(void) const
{
    return this->id;
}

template <typename T>
void Node<T>::setOccupier(T occ)
{
    this->occupier = occ;
}

template <typename T>
T Node<T>::getOccupier(void) const
{
    return this->occupier;
}

template <typename U>
ostream& operator<< (ostream& out, const Node<U>& n)
{
    return out << n.occupier;
}

template <typename U>
ostream& operator<< (ostream& out, const Node<U>*& n)
{
    return out << n->occupier;
}