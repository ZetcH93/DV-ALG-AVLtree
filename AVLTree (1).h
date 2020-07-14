#ifndef AVLTREE_H
#define AVLTREE_H

#include <memory>
#include <cstdlib>
#include <utility>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <vector>

#include <string> // för att få to_string att fungera, Robin Lenz sa att detta va okej om man hade problem

template < typename ElementValue>
class AVLTree
{
	public:
		
		AVLTree();
		AVLTree(const AVLTree& rhs);
		~AVLTree();
		ElementValue& getMin() const;								//skickar tillbaka det minsta värdet i trädet
		ElementValue& getMax() const;								//skickar tillbaka det största värdet i trädet
		bool find(const ElementValue& x) const;						// returnar true/false efter att ha sökt efter en nod av värdet x.
		bool isEmpty() const;										//returnar true om trädet är tomt.
		void makeEmpty();											//tömmer trädet på noder genom rekursion.
		void insert(const ElementValue& x);							//sätt in ett nytt värde(nod) "x", balanserar trädet.
		void remove(const ElementValue& x);							// tar bort värdet(nod) "x", och balanserar trädet.
		size_t getTreeHeight() const;								//skickar tillbaka trädets höjd, dvs rootens höjd, -1 om trädet är tomt.
		std::vector<ElementValue> preOrderWalk();					// skapar en vector och fyller den med trädets element genom en preorder travesering rekursion.
		std::vector<ElementValue> inOrderWalk();					// skapar en vector och fyller den med trädets element genom en inorder travesering rekursion.
		std::vector<ElementValue> postOrderWalk();					// skapar en vector och fyller den med trädets element genom en postorder travesering rekursion.
		class UnderflowException {};								// ger en underflowexception.
		std::string ToGraphviz();									// graphviz funktion.
		const AVLTree<ElementValue>& operator=(const AVLTree& rhs);	//deep copy.
	

		 			
	private:
		struct AVLNode		//innehåller data för varje inviduel node i trädet.
		{
			 ElementValue element;
			 AVLNode *left = nullptr;
			 AVLNode *right = nullptr;
			 int height;
			
			 AVLNode(const ElementValue & ele, AVLNode * lt, AVLNode * rt, int h = 0)
			   : element( ele ), left( lt ), right( rt ), height( h ) { }
		};
	

		AVLNode* root;									//trädets root.
		static const int ALLOWED_IMBALANCE = 1;			//används när balansering utförs vid insättning.

		void insert(const ElementValue & x, AVLNode* &t);		//hittar den plats där noden ska läggas in genom rekursion, sätter sedan in och utför balansering.
		void remove(const ElementValue & x, AVLNode* &t);		//hittar det givna värdet genom rekursion och tar sedan bort -> balansering.
		bool find(const ElementValue & x, AVLNode* t) const;	//hittar den eftersökta noden genom rekursion.
		void makeEmpty(AVLNode*& t);					// tömmer trädet genom rekursion.
		int height(AVLNode* t) const;					//skickar tillbaka höjden för noden i parametern.
		void balance(AVLNode*& t);						//balansering utav trädet, används efter att remove funktionen har användts.
		void doubleWithRightChild(AVLNode*& t);			// utför en dubbelrotation med höger barn.
		void doubleWithLeftChild(AVLNode*& t);			// utför en dubbelrotation med vänster barn.
		void rotateWithRightChild(AVLNode*& t);			// utför en enkelrotation med höger barn.
		void rotateWithLeftChild(AVLNode*& t);			// utför en enkelrotation med vänster barn.
		int max(int lhs, int rhs) const;				// kollar två noders höjd och returnerar det högsta värdet.
		void preOrderWalk(AVLNode*& t, std::vector<ElementValue>& pre);	
		void inOrderWalk(AVLNode*& t, std::vector<ElementValue>& in);
		void postOrderWalk(AVLNode*& t, std::vector<ElementValue>& post);
		void ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, AVLNode* toWorkWith, size_t& uniqueID);
		AVLNode* getMin(AVLNode* t) const;				//hittar minsta värdet genom rekursion.
		AVLNode* getMax(AVLNode* t) const;				//hittar största värdet genom rekursion.
		AVLNode* clone(AVLNode* t) const;				//används vid deep copy.
	
		
	
	
};

template <typename ElementValue>
AVLTree<ElementValue>::AVLTree() :root(NULL)
{
}

template <typename ElementValue>
AVLTree<ElementValue>::AVLTree(const AVLTree& rhs) : root(NULL)
{
	*this = rhs;
}


template <typename ElementValue>
AVLTree<ElementValue>::~AVLTree()
{
	makeEmpty();
}

template <typename ElementValue>
ElementValue& AVLTree<ElementValue>::getMin() const
{
	if (isEmpty())
		throw UnderflowException();
	return getMin(root)->element;
}

template <typename ElementValue>
ElementValue& AVLTree<ElementValue>::getMax() const
{
	if (isEmpty())
		throw UnderflowException();
	return getMax(root)->element;
}

template <typename ElementValue>
bool AVLTree<ElementValue>::find(const ElementValue& x) const
{
	return find(x, root);
}


template <typename ElementValue>
bool AVLTree<ElementValue>::isEmpty() const
{
	return root == NULL;
}




template <typename ElementValue>
void AVLTree<ElementValue>::makeEmpty()
{
	makeEmpty(root);
}


template <typename ElementValue>
void AVLTree<ElementValue>::insert(const ElementValue& x)
{
	insert(x, root);
}



template <typename ElementValue>
void AVLTree<ElementValue>::remove(const ElementValue& x)
{
	remove(x, root);
}


template <typename ElementValue>
size_t AVLTree<ElementValue>::getTreeHeight() const
{	
	return static_cast<size_t>(height(root));
}

template <typename ElementValue>
void AVLTree<ElementValue>::makeEmpty(AVLNode*& t)
{
	if (t != nullptr)
	{
		makeEmpty(t->left);
		makeEmpty(t->right);
		delete t;
	}
	t = nullptr;
}

template<typename ElementValue>
const AVLTree<ElementValue>& AVLTree<ElementValue>::operator=(const AVLTree& rhs)
{
	if (this != &rhs)
	{
		makeEmpty();
		root = clone(rhs.root);
	}
	return *this;
}

template <typename ElementValue>
typename AVLTree<ElementValue>::AVLNode* AVLTree<ElementValue>::getMin(AVLNode* t) const
{
	if (t == nullptr)
		return nullptr;
	if (t->left == nullptr)
		return t;
	return getMin(t->left);
}

template <typename ElementValue>
typename AVLTree<ElementValue>::AVLNode* AVLTree<ElementValue>::getMax(AVLNode* t) const
{
	if (t == nullptr)
		return nullptr;
	if (t->right == nullptr)
		return t;
	return getMax(t->right);
}

template <typename ElementValue>
typename AVLTree<ElementValue>::AVLNode* AVLTree<ElementValue>::clone(AVLNode* t) const
{
	if (t == NULL)
		return NULL;
	else
		return new AVLNode(t->element, clone(t->left), clone(t->right), t->height);
}



template <typename ElementValue>
void AVLTree<ElementValue>::insert(const ElementValue& x, AVLNode*& t)
{
	if (t == NULL)
		t = new AVLNode(x, NULL, NULL);
	else if (x < t->element)
	{
		insert(x, t->left);
		if (height(t->left) - height(t->right) == 2)
			if (x < t->left->element)
				rotateWithLeftChild(t);
			else
				doubleWithLeftChild(t);
	}
	else if (t->element < x)
	{
		insert(x, t->right);
		if (height(t->right) - height(t->left) == 2)
			if (t->right->element < x)
				rotateWithRightChild(t);
			else
				doubleWithRightChild(t);
	}
	else
		;	//nod med detta värdet finns redan, inget görs
	t->height = max(height(t->left), height(t->right)) + 1;
}


template <typename ElementValue>
void AVLTree<ElementValue>::balance(AVLNode*& t)
{
	if (t == NULL)
		return;
	
	if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)
		if (height(t->left->left) >= height(t->left->right))
			rotateWithLeftChild(t);
		else
			doubleWithLeftChild(t);
	else
	if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE)
		if (height(t->right->right) >= height(t->right->left))
			rotateWithRightChild(t);
		else
			doubleWithRightChild(t);
	
	t->height = max(height(t->left), height(t->right)) + 1;
}

template <typename ElementValue>
void AVLTree<ElementValue>::remove(const ElementValue& x, AVLNode*& t)
{
	if (t == nullptr)
		return;			// noden finns inte, inget görs
	if (x < t->element)
		remove(x, t->left);
	else if (t->element < x)
		remove(x, t->right);
	else if (t->left != nullptr && t->right != nullptr) // noden har två stycken barn
	{
		t->element = getMin(t->right)->element;
		remove(t->element, t->right);
	}
	else
	{
		AVLNode * oldNode = t;
		t = (t->left != nullptr) ? t->left : t->right;
		delete oldNode;
	 }

	balance(t);
}


template <typename ElementValue>
bool AVLTree<ElementValue>::find(const ElementValue& x, AVLNode* t) const
{
	if (t == nullptr)
		return false;				//värdet finns inte
	else if (x < t->element)
		return find(x, t->left);
	else if (t->element < x)
		return find(x, t->right);
	else
		return true; 

}


template <typename ElementValue>
int AVLTree<ElementValue>::height(AVLNode* t) const
{
	return t == NULL ? -1 : t->height;		//skickar tbx -1 om trädet är tomt
}


template <typename ElementValue>
void AVLTree<ElementValue>::doubleWithRightChild(AVLNode*& t)
{
	rotateWithLeftChild(t->right);
	rotateWithRightChild(t);
}
template <typename ElementValue>
void AVLTree<ElementValue>::doubleWithLeftChild(AVLNode*& t)
{
	rotateWithRightChild(t->left);
	rotateWithLeftChild(t);
}
template <typename ElementValue>
void AVLTree<ElementValue>::rotateWithRightChild(AVLNode*& t)
{

	AVLNode* node = t->right;
	t->right = node->left;
	node->left = t;
	t->height = max(height(t->left), height(t->right)) + 1;
	node->height = max(height(node->right), t->height) + 1;
	t = node;

}
template <typename ElementValue>
void AVLTree<ElementValue>::rotateWithLeftChild(AVLNode*& t)
{	
	
	AVLNode* node = t->left;
	t->left = node->right;
	node->right = t;
	t->height = max(height(t->left), height(t->right)) + 1;
	node->height = max(height(node->left), t->height) + 1;
	t = node;
	
	
}
template <typename ElementValue>
int AVLTree<ElementValue>::max(int lhs, int rhs) const
{
	return lhs > rhs ? lhs : rhs;
}

template <typename ElementValue>
std::vector<ElementValue> AVLTree<ElementValue>::preOrderWalk()
{
	std::vector<ElementValue> pre;
	preOrderWalk(root, pre);
	return pre;
}

template <typename ElementValue>
void AVLTree<ElementValue>::preOrderWalk(AVLNode*& t, std::vector<ElementValue>& pre)
{
	if (t != nullptr)
	{
		pre.push_back(t->element);
		preOrderWalk(t->left, pre);
		preOrderWalk(t->right, pre);
	}
}

template <typename ElementValue>
std::vector<ElementValue> AVLTree<ElementValue>::inOrderWalk()
{
	std::vector<ElementValue> in;
	inOrderWalk(root,in);
	return in;
}

template <typename ElementValue>
void AVLTree<ElementValue>::inOrderWalk(AVLNode*& t, std::vector<ElementValue>& in)
{
	if (t != nullptr)
	{
		inOrderWalk(t->left, in);
		in.push_back(t->element);
		inOrderWalk(t->right, in);
	}
}

template <typename ElementValue>
std::vector<ElementValue> AVLTree<ElementValue>::postOrderWalk()
{
	std::vector<ElementValue> post;
	postOrderWalk(root, post);
	return post;
}

template <typename ElementValue>
void AVLTree<ElementValue>::postOrderWalk(AVLNode*& t, std::vector<ElementValue>& post)
{
	if (t != nullptr)
	{
		postOrderWalk(t->left, post);
		postOrderWalk(t->right, post);
		post.push_back(t->element);
	}
}




template <typename ElementValue>
std::string AVLTree<ElementValue>::ToGraphviz() // Member function of the AVLTree class
{
	std::string toReturn = "";
	if(root) // root is a pointer to the root node of the tree
	{
		std::string listOfNodes;
		std::string listOfConnections = std::string("\t\"Root\" -> ") + std::to_string(0) + std::string(";\n");
		toReturn += std::string("digraph {\n");
		size_t id = 0;
		ToGraphvizHelper(listOfNodes, listOfConnections, root, id);
		toReturn += listOfNodes;
		toReturn += listOfConnections;
		toReturn += std::string("}");
	}
	return move(toReturn);
}







template <typename ElementValue>
void AVLTree<ElementValue>::ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, AVLNode* toWorkWith, size_t& uniqueID) // Member function of the AVLTree class
{
	size_t myID = uniqueID;
	listOfNodes += std::string("\t") + std::to_string(myID) + std::string(" [label=\"") + std::to_string(toWorkWith->element) + std::string("\"];\n");
	if (toWorkWith->left)
	{
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID + 1) + std::string(" [color=blue];\n");
		ToGraphvizHelper(listOfNodes, listOfConnections, toWorkWith->left, ++uniqueID);
	}
	else
	{
		listOfNodes += std::string("\t") + std::to_string(++uniqueID) + std::string(" [label=") + std::string("nill, style = invis];\n");
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID) + std::string(" [ style = invis];\n");
	}

	if (toWorkWith->right)
	{
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID + 1) + std::string(" [color=red];\n");
		ToGraphvizHelper(listOfNodes, listOfConnections, toWorkWith->right, ++uniqueID);
	}
	else
	{
		listOfNodes += std::string("\t") + std::to_string(++uniqueID) + std::string(" [label=") + std::string("nill, style = invis];\n");
		listOfConnections += std::string("\t") + std::to_string(myID) + std::string(" -> ") + std::to_string(uniqueID) + std::string(" [ style = invis];\n");
	}
}



#endif