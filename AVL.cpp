#include <iostream>
using namespace std;
#define LH +1
#define EH  0
#define RH -1

template <class T>
struct node
{
	T data;
	node* left;
	node* right;
	int bal;
};

template <class T, class V>
class Tree
{
private:
	int count;
	node<T>* tree;
	node<T>* _insert(node<T>* root, node<T>* newPtr, bool& taller);
	node<T>* leftBalance(node<T>* root, bool& taller);
	node<T>* rotateLeft(node<T>* root);
	node<T>* rightBalance(node<T>* root, bool& taller);
	node<T>* rotateRight(node<T>* root);
	node<T>* _delete(node<T>* root, V dltKey, bool& shorter, bool& success);
	node<T>* dltLeftBalance(node<T>* root, bool& smaller);
	node<T>* dltRightBalance(node<T>* root, bool& shorter);
	node<T>* _retrieve(V key, node<T>* root);
	void  _traversal(void(*process)(T dataProc), node<T>* root);
	void  _traversal_preorder(void(*process)(T dataProc), node<T>* root);
	void  _traversal_postorder(void(*process)(T dataProc), node<T>* root);
	void  _destroy(node<T>* root);
	void  _print(node<T>* root, int level);

public:
	Tree(void);
	~Tree(void);
	bool  _Insert(T   dataIn);
	bool  _Delete(V  dltKey);
	node<T>* _Retrieve2(V  key, T& dataOut);
	bool  _Retrieve(V  key, T& dataOut);
	void  _Traverse(void(*process)(T  dataProc));
	void  _Traverse_preorder(void(*process)(T  dataProc));
	void  _Traverse_postorder(void(*process)(T  dataProc));
	bool  _Empty(void);
	bool  _Full(void);
	int   _Count(void);
	void  _Print(void);
};

template <class T, class V>
Tree<T, V> ::Tree(void)
{
	tree = NULL;
	count = 0;
}

template <class T, class V>
bool  Tree<T, V> ::_Insert(T dataIn)
{
	node<T>* newPtr;
	bool taller;
	if (!(newPtr = new node<T>))return false;
	newPtr->bal = EH;
	newPtr->right = NULL;
	newPtr->left = NULL;
	newPtr->data = dataIn;
	tree = _insert(tree, newPtr, taller);
	count++;
	return true;
}

template <class T, class V>
node<T>* Tree<T, V>::_insert(node<T>* root, node<T>* newPtr, bool& taller)
{
	if (!root)
	{
		root = newPtr;
		taller = true;
		return  root;
	}
	if (newPtr->data.key < root->data.key)
	{
		root->left = _insert(root->left, newPtr, taller);
		if (taller)
			switch (root->bal)
			{
			case LH: root = leftBalance(root, taller); break;
			case EH: root->bal = LH; break;
			case RH: root->bal = EH; taller = false; break;
			}
	}
	else
	{
		root->right = _insert(root->right, newPtr, taller);
		if (taller)
			switch (root->bal)
			{
			case LH:root->bal = EH; taller = false; break;
			case EH:root->bal = RH; break;
			case RH:root = rightBalance(root, taller); break;
			}
	}
	return root;
}

template <class T, class V>
node<T>* Tree<T, V>::leftBalance(node<T>* root, bool& taller)
{
	node<T>* rightTree;
	node<T>* leftTree;
	leftTree = root->left;
	switch (leftTree->bal)
	{
	case LH:root->bal = EH; leftTree->bal = EH; root = rotateRight(root); taller = false; break;
	case EH:cout << "\n\a\aError in leftBalance\n"; exit(100);
	case RH:rightTree = leftTree->right;
		switch (rightTree->bal)
		{
		case LH: root->bal = RH; leftTree->bal = EH; break;
		case EH: root->bal = EH; leftTree->bal = EH; break;
		case RH: root->bal = EH; leftTree->bal = LH; break;
		}
		rightTree->bal = EH; root->left = rotateLeft(leftTree); root = rotateRight(root); taller = false;
	}
	return root;
}

template <class T, class V>
node<T>* Tree<T, V>::rotateLeft(node<T>* root)
{
	node<T>* tempPtr;
	tempPtr = root->right;
	root->right = tempPtr->left;
	tempPtr->left = root;
	return tempPtr;
}

template <class T, class V>node<T>* Tree<T, V>::rotateRight(node<T>* root)
{
	node<T>* tempPtr;
	tempPtr = root->left;
	root->left = tempPtr->right;
	tempPtr->right = root;
	return tempPtr;
}

template <class T, class V>node<T>* Tree<T, V>::rightBalance(node<T>* root, bool& taller)
{
	node<T>* rightTree;
	node<T>* leftTree;
	rightTree = root->right;
	switch (rightTree->bal)
	{
	case LH:leftTree = rightTree->left;
		switch (leftTree->bal)
		{
		case RH: root->bal = LH; rightTree->bal = EH; break;
		case EH: root->bal = EH; rightTree->bal = EH; break;
		case LH: root->bal = EH; rightTree->bal = RH; break;
		}
		leftTree->bal = EH;
		root->right = rotateRight(rightTree);
		root = rotateLeft(root);
		taller = false;
		break;
	case EH:root->bal = EH; taller = false; break;
	case RH:root->bal = EH; rightTree->bal = EH; root = rotateLeft(root); taller = false; break;
	}
	return root;
}

template <class T, class V>
bool  Tree <T, V> ::_Delete(V  dltKey)
{
	bool shorter;
	bool success;
	node<T>* newRoot;
	newRoot = _delete(tree, dltKey, shorter, success);
	if (success)
	{
		tree = newRoot;
		count--;
	}
	return success;
}

template <class T, class V>
node<T>* Tree<T, V>::_delete(node<T>* root, V       dltKey, bool& shorter, bool& success)
{
	node<T>* dltPtr;
	node<T>* exchPtr;
	node<T>* newRoot;
	if (!root)
	{
		shorter = false;
		success = false;
		return NULL;
	}
	if (dltKey < root->data.key)
	{
		root->left = _delete(root->left, dltKey, shorter, success);
		if (shorter)
			root = dltRightBalance(root, shorter);
	}
	else if (dltKey > root->data.key)
	{
		root->right = _delete(root->right, dltKey, shorter, success);
		if (shorter)
			root = dltLeftBalance(root, shorter);
	}
	else
	{
		dltPtr = root;
		if (!root->right)
		{
			newRoot = root->left;
			success = true;
			shorter = true;
			delete (dltPtr);
			return newRoot;
		}
		else
			if (!root->left)
			{
				newRoot = root->right;
				success = true;
				shorter = true;
				delete (dltPtr);
				return newRoot;
			}
			else
			{
				exchPtr = root->left;
				while (exchPtr->right) exchPtr = exchPtr->right;
				root->data = exchPtr->data;
				root->left = _delete(root->left, exchPtr->data.key, shorter, success);
				if (shorter) root = dltRightBalance(root, shorter);
			}
	}
	return root;
}

template <class T, class V>
node<T>* Tree<T, V>::dltLeftBalance(node<T>* root, bool& smaller)
{
	node<T>* rightTree;
	node<T>* leftTree;
	switch (root->bal)
	{
	case RH:  root->bal = EH; break;
	case EH:root->bal = LH; smaller = false; break;
	case LH:  leftTree = root->left;
		switch (leftTree->bal)
		{
		case LH:
		case EH:
			if (leftTree->bal == EH)
			{
				root->bal = LH;
				leftTree->bal = RH;
				smaller = false;
			}
			else
			{
				root->bal = EH;
				leftTree->bal = EH;
			}
			root = rotateRight(root);
			break;
		case RH:rightTree = leftTree->right;
			switch (rightTree->bal)
			{
			case LH: root->bal = RH; leftTree->bal = EH; break;
			case EH: root->bal = EH; leftTree->bal = EH; break;
			case RH: root->bal = EH; leftTree->bal = LH; break;
			}
			rightTree->bal = EH;
			root->left = rotateLeft(leftTree);
			root = rotateRight(root);
			break;
		}
	}
	return root;
}

template <class T, class V>node<T>* Tree<T, V>::dltRightBalance(node<T>* root, bool& shorter)
{
	node<T>* rightTree;
	node<T>* leftTree;
	switch (root->bal)
	{
	case LH:root->bal = EH; break;
	case EH:root->bal = RH; shorter = false; break;
	case RH:rightTree = root->right;
		if (rightTree->bal == LH)
		{
			leftTree = rightTree->left;
			switch (leftTree->bal)
			{
			case LH: rightTree->bal = RH; root->bal = EH; break;
			case EH: root->bal = EH; rightTree->bal = EH; break;
			case RH: root->bal = LH; rightTree->bal = EH; break;
			}
			leftTree->bal = EH;
			root->right = rotateRight(rightTree);
			root = rotateLeft(root);
		}
		else
		{
			switch (rightTree->bal)
			{
			case LH:case RH: root->bal = EH; rightTree->bal = EH; break;
			case EH: root->bal = RH; rightTree->bal = LH; shorter = false; break;
			}
			root = rotateLeft(root);
		}
	}
	return root;
}

template <class T, class V>	bool Tree<T, V>::_Retrieve(V   key, T& dataOut)
{
	node<T>* node;
	if (!tree)return false;
	node = _retrieve(key, tree);
	if (node)
	{
		dataOut = node->data;
		return true;
	}
	else return false;
}

template <class T, class V> node<T>* Tree<T, V>::_Retrieve2(V   key, T& dataOut)
{
	node<T>* node;
	if (!tree)return NULL;
	node = _retrieve(key, tree);
	if (node)
	{
		return node;
	}
	else return NULL;
}

template <class T, class V>node<T>* Tree<T, V>::_retrieve(V       key, node<T>* root)
{
	if (root)
	{
		if (key < root->data.key) return _retrieve(key, root->left);
		else if (key > root->data.key) return _retrieve(key, root->right);
		else return (root);
	}
	else return root;
}

template <class T, class V>void  Tree<T, V>::_Traverse(void(*process)(T dataProc))
{
	_traversal(process, tree); return;
}

template <class T, class V> void  Tree<T, V>::_Traverse_preorder(void(*process)(T dataProc))
{
	_traversal_preorder(process, tree);
	return;
}

template <class T, class V>void  Tree<T, V>::_Traverse_postorder(void(*process)(T dataProc))
{
	_traversal_postorder(process, tree);
	return;
}

template <class T, class V>
void  Tree<T, V>::_traversal(void(*process)(T dataproc), node<T>* root)
{
	if (root)
	{
		_traversal(process, root->left);
		process(root->data);
		_traversal(process, root->right);
	}
	return;
}

template <class T, class V>
void  Tree<T, V>::_traversal_preorder(void(*process)(T dataproc), node<T>* root)
{
	if (root)
	{
		process(root->data);
		_traversal_preorder(process, root->left);
		_traversal_preorder(process, root->right);
	}
	return;
}

template <class T, class V>
void  Tree<T, V>::_traversal_postorder(void(*process)(T dataproc), node<T>* root)
{
	if (root)
	{
		_traversal_preorder(process, root->left);
		_traversal_preorder(process, root->right);
		process(root->data);
	}
	return;
}

template <class T, class V>
bool   Tree<T, V> ::_Empty(void)
{
	return (count == 0);
}

template <class T, class V>
bool   Tree<T, V> ::_Full(void)
{
	node<T>* newPtr;
	newPtr = new node<T>;
	if (newPtr)
	{
		delete  newPtr; return false;
	}
	else return true;
}

template <class T, class V>
int  Tree<T, V> ::_Count(void)
{
	return (count);
}

template <class T, class V>
Tree<T, V> :: ~Tree(void)
{
	if (tree) _destroy(tree);
}

template <class T, class V>void  Tree<T, V>::_destroy(node<T>* root)
{
	if (root)
	{
		_destroy(root->left);
		_destroy(root->right);
		delete root;
	}
	return;
}

template <class T, class V>
void  Tree<T, V> ::_Print(void)
{
	_print(tree, 0);
	return;
}

template <class T, class V>void  Tree<T, V> ::_print(node<T>* root, int level)
{
	int i;
	if (root)
	{
		_print(root->right, level + 1);
		cout << "bal " << root->bal
			<< ": Level " << level;
		for (i = 0; i <= level; i++)
			cout << "....";
		cout << root->data.key;
		if (root->bal == LH)
			cout << " (LH)\n";
		else if (root->bal == RH)
			cout << " (RH)\n";
		else
			cout << " (EH)\n";
		_print(root->left, level + 1);
	}
}

