#ifndef AVLTREE_H
#define AVLTREE_H

#include <memory>
#include <cstdlib>
#include <utility>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <vector>

#include <string> // f�r att f� to_string att fungera, Robin Lenz sa att detta va okej om man hade problem

template < typename ElementValue>
class AVLTree
{
	public:
		
		AVLTree();
		AVLTree(const AVLTree& rhs);
		~AVLTree();
		ElementValue& getMin() const;								//skickar tillbaka det minsta v�rdet i tr�det
		ElementValue& getMax() const;								//skickar tillbaka det st�rsta v�rdet i tr�det
		bool find(const ElementValue& x) const;						// returnar true/false efter att ha s�kt efter en nod av v�rdet x.
		bool isEmpty() const;										//returnar true om tr�det �r tomt.
		void makeEmpty();											//t�mmer tr�det p� noder genom rekursion.
		void insert(const ElementValue& x);							//s�tt in ett nytt v�rde(nod) "x", balanserar tr�det.
		void remove(const ElementValue& x);							// tar bort v�rdet(nod) "x", och balanserar tr�det.
		size_t getTreeHeight() const;								//skickar tillbaka tr�dets h�jd, dvs rootens h�jd, -1 om tr�det �r tomt.
		std::vector<ElementValue> preOrderWalk();					// skapar en vector och fyller den med tr�dets element genom en preorder travesering rekursion.
		std::vector<ElementValue> inOrderWalk();					// skapar en vector och fyller den med tr�dets element genom en inorder travesering rekursion.
		std::vector<ElementValue> postOrderWalk();					// skapar en vector och fyller den med tr�dets element genom en postorder travesering rekursion.
		class UnderflowException {};								// ger en underflowexception.
		std::string ToGraphviz();									// graphviz funktion.
		const AVLTree<ElementValue>& operator=(const AVLTree& rhs);	//deep copy.
	

		 			
	private:
		struct AVLNode		//inneh�ller data f�r varje inviduel node i tr�det.
		{
			 ElementValue element;
			 AVLNode *left = nullptr;
			 AVLNode *right = nullptr;
			 int height;
			
			 AVLNode(const ElementValue & ele, AVLNode * lt, AVLNode * rt, int h = 0)
			   : element( ele ), left( lt ), right( rt ), height( h ) { }
		};
	

		AVLNode* root;									//tr�dets root.
		static const int ALLOWED_IMBALANCE = 1;			//anv�nds n�r balansering utf�rs vid ins�ttning.

		void insert(const ElementValue & x, AVLNode* &t);		//hittar den plats d�r noden ska l�ggas in genom rekursion, s�tter sedan in och utf�r balansering.
		void remove(const ElementValue & x, AVLNode* &t);		//hittar det givna v�rdet genom rekursion och tar sedan bort -> balansering.
		bool find(const ElementValue & x, AVLNode* t) const;	//hittar den efters�kta noden genom rekursion.
		void makeEmpty(AVLNode*& t);					// t�mmer tr�det genom rekursion.
		int height(AVLNode* t) const;					//skickar tillbaka h�jden f�r noden i parametern.
		void balance(AVLNode*& t);						//balansering utav tr�det, anv�nds efter att remove funktionen har anv�ndts.
		void doubleWithRightChild(AVLNode*& t);			// utf�r en dubbelrotation med h�ger barn.
		void doubleWithLeftChild(AVLNode*& t);			// utf�r en dubbelrotation med v�nster barn.
		void rotateWithRightChild(AVLNode*& t);			// utf�r en enkelrotation med h�ger barn.
		void rotateWithLeftChild(AVLNode*& t);			// utf�r en enkelrotation med v�nster barn.
		int max(int lhs, int rhs) const;				// kollar tv� noders h�jd och returnerar det h�gsta v�rdet.
		void preOrderWalk(AVLNode*& t, std::vector<ElementValue>& pre);	
		void inOrderWalk(AVLNode*& t, std::vector<ElementValue>& in);
		void postOrderWalk(AVLNode*& t, std::vector<ElementValue>& post);
		void ToGraphvizHelper(std::string& listOfNodes, std::string& listOfConnections, AVLNode* toWorkWith, size_t& uniqueID);
		AVLNode* getMin(AVLNode* t) const;				//hittar minsta v�rdet genom rekursion.
		AVLNode* getMax(AVLNode* t) const;				//hittar st�rsta v�rdet genom rekursion.
		AVLNode* clone(AVLNode* t) const;				//anv�nds vid deep copy.
	
		
	
	
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
		;	//nod med detta v�rdet finns redan, inget g�rs
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
		return;			// noden finns inte, inget g�rs
	if (x < t->element)
		remove(x, t->left);
	else if (t->element < x)
		remove(x, t->right);
	else if (t->left != nullptr && t->right != nullptr) // noden har tv� stycken barn
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
		return false;				//v�rdet finns inte
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
	return t == NULL ? -1 : t->height;		//skickar tbx -1 om tr�det �r tomt
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