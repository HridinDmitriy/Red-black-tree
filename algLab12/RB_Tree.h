#pragma once
#include <string>

class KeyNotFoundException {};

template <typename TKey, typename TValue>
class RB_Tree
{
public:
	RB_Tree();
	RB_Tree(const TKey& key, const TValue& data);
	RB_Tree(const RB_Tree<TKey, TValue>& anotherTree);
	~RB_Tree();
	TValue& operator[](const TKey& key);
	RB_Tree<TKey, TValue>& operator=(const RB_Tree<TKey, TValue>& anotherTree);
	int get_size() { return size; }
	void Add(const TKey& key, const TValue& data);
	void Delete(const TKey& key);
	bool ContainsKey(const TKey& key);
	TValue& TryGetValue(const TKey& key);
	void print();
	void clear();
private:
	template <typename TKey, typename TValue>
	struct Node
	{
		Node* parent;
		Node* left;
		Node* right;

		TKey key;
		TValue data;
		bool color;   //1 - black, 0 - red
		int black_left;
		int black_right;

		Node(const TKey& _key = TKey(), const TValue& _data = TValue(), Node* _parent = nullptr, bool _color = false)
			: key(_key), data(_data), parent(_parent), left(nullptr), right(nullptr), color(_color), black_left(0), black_right(0)
		{ }

		~Node()
		{
			delete left;
			delete right;
		}
	};
	int size;
	Node<TKey, TValue>* root;
	void PostOrderTreeWalk(Node<TKey, TValue>* node);
	Node<TKey, TValue>* TreeSearch(Node<TKey, TValue>* node, const TKey& key);
	void Transplant(Node<TKey, TValue>* source, Node<TKey, TValue>* target);
	Node<TKey, TValue>* MinimumKeyNode(Node<TKey, TValue>* node);
	Node<TKey, TValue>* GetCopy(Node<TKey, TValue>* node, Node<TKey, TValue>* parent);
	void rotate_left(Node<TKey, TValue>* node);
	void rotate_right(Node<TKey, TValue>* node);
	void BalanceAfterInsertion(Node<TKey, TValue>* node);
	void BalanceAfterRemoval(Node<TKey, TValue>* node, Node<TKey, TValue>* _parent);
};

template<typename TKey, typename TValue>
inline RB_Tree<TKey, TValue>::RB_Tree()
	: size(0), root(nullptr)
{ }

template<typename TKey, typename TValue>
inline RB_Tree<TKey, TValue>::RB_Tree(const TKey& rootKey, const TValue& rootData)
	: RB_Tree<TKey, TValue>::RB_Tree()
{
	root = new Node<TKey, TValue>(rootKey, rootData);
	size++;
}

template<typename TKey, typename TValue>
inline RB_Tree<TKey, TValue>::RB_Tree(const RB_Tree<TKey, TValue>& anotherTree)
{
	size = anotherTree.size;
	root = GetCopy(anotherTree.root, nullptr); 
}

template<typename TKey, typename TValue>
inline RB_Tree<TKey, TValue>::~RB_Tree()
{
	clear();
}

template<typename TKey, typename TValue>
inline TValue& RB_Tree<TKey, TValue>::operator[](const TKey& key)
{
	return TryGetValue(key);
}

template<typename TKey, typename TValue>
inline RB_Tree<TKey, TValue>& RB_Tree<TKey, TValue>::operator=(const RB_Tree<TKey, TValue>& anotherTree)
{
	if (this != &anotherTree)
	{
		this->clear();
		size = anotherTree.size;
		root = GetCopy(anotherTree.root, nullptr); 
	}
	return *this;
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::Add(const TKey& key, const TValue& data)
{
	if (ContainsKey(key))
		return;

	Node<TKey, TValue>* toInsert = new Node<TKey, TValue>(key, data);
	if (root == nullptr)
	{
		root = toInsert;
	}
	else
	{
		Node<TKey, TValue>* parent = nullptr;
		Node<TKey, TValue>* current = root;

		while (current != nullptr)
		{
			parent = current;
			if (toInsert->key < current->key)
				current = current->left;
			else current = current->right;
		}
		toInsert->parent = parent;

		if (toInsert->key < parent->key)
			parent->left = toInsert;
		else parent->right = toInsert;
	}
	size++;
	BalanceAfterInsertion(toInsert);
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::Delete(const TKey& key)
{
	Node<TKey, TValue>* toDelete = TreeSearch(root, key);
	if (toDelete == nullptr)
		return;

	Node<TKey, TValue>* willCauseTheImbalance = toDelete;
	bool originalColor = willCauseTheImbalance->color;
	Node<TKey, TValue>* imbalanceTreeRoot; 
	Node<TKey, TValue>* parent;

	if (toDelete->left == nullptr)
	{
		imbalanceTreeRoot = toDelete->right;
		parent = toDelete->parent;
		Transplant(toDelete->right, toDelete); 
	}
	else if (toDelete->right == nullptr)
	{
		imbalanceTreeRoot = toDelete->left;
		parent = toDelete->parent;
		Transplant(toDelete->left, toDelete);
	}
	else
	{
		willCauseTheImbalance = MinimumKeyNode(toDelete->right);
		originalColor = willCauseTheImbalance->color;
		imbalanceTreeRoot = willCauseTheImbalance->right;
		parent = willCauseTheImbalance;
		if (willCauseTheImbalance->parent != toDelete)
		{
			parent = willCauseTheImbalance->parent;
			Transplant(willCauseTheImbalance->right, willCauseTheImbalance);
			willCauseTheImbalance->right = toDelete->right;
			willCauseTheImbalance->right->parent = willCauseTheImbalance;
		}
		Transplant(willCauseTheImbalance, toDelete);
		willCauseTheImbalance->left = toDelete->left;
		willCauseTheImbalance->left->parent = willCauseTheImbalance;
		willCauseTheImbalance->color = toDelete->color;
	}

	if (originalColor == true)
	{
		BalanceAfterRemoval(imbalanceTreeRoot, parent);
	}

	toDelete->left = nullptr;
	toDelete->right = nullptr;
	delete toDelete;
	size--;
}

template<typename TKey, typename TValue>
inline TValue& RB_Tree<TKey, TValue>::TryGetValue(const TKey& key)
{
	Node<TKey, TValue>* node = TreeSearch(root, key);
	if (node != nullptr)
		return node->data;
	else
		throw KeyNotFoundException();
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::Transplant(Node<TKey, TValue>* source, Node<TKey, TValue>* target)
{
	if (target->parent == nullptr)
		root = source;
	else if (target == target->parent->left)
		target->parent->left = source;
	else target->parent->right = source;

	if (source != nullptr)
		source->parent = target->parent;
}

template<typename TKey, typename TValue>
inline RB_Tree<TKey, TValue>::Node<TKey, TValue>* RB_Tree<TKey, TValue>::MinimumKeyNode(Node<TKey, TValue>* node)
{
	while (node->left != nullptr)
	{
		node = node->left;
	}
	return node;
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::PostOrderTreeWalk(Node<TKey, TValue>* node)
{
	if (node != nullptr)
	{
		PostOrderTreeWalk(node->left);
		PostOrderTreeWalk(node->right);
		node->black_left = 0;
		node->black_right = 0;
		if (node->left) node->black_left += node->left->black_left; 
		if (node->right) node->black_right += node->right->black_right;
		if (node->color)
		{
			node->black_left++;
			node->black_right++;
		}

		std::cout << node->key << " : " << node->data << "\t" << node->black_left << " : " << node->black_right;// << std::endl;
		if (node->black_left == node->black_right) std::cout << "\tok" << std::endl;
		else std::cout << "\terror" << std::endl;
	}
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::print()
{
	PostOrderTreeWalk(root);
}

template<typename TKey, typename TValue>
inline RB_Tree<TKey, TValue>::Node<TKey, TValue>* RB_Tree<TKey, TValue>::TreeSearch(Node<TKey, TValue>* node, const TKey& key)
{
	while (node != nullptr && key != node->key)
	{
		if (key < node->key)
			node = node->left;
		else node = node->right;
	}
	return node;
}

template<typename TKey, typename TValue>
inline bool RB_Tree<TKey, TValue>::ContainsKey(const TKey& key)
{
	return TreeSearch(root, key) ? true : false;
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::clear()
{
	delete root;
}

template<typename TKey, typename TValue>
inline RB_Tree<TKey, TValue>::Node<TKey, TValue>* RB_Tree<TKey, TValue>::GetCopy(Node<TKey, TValue>* oldnode, Node<TKey, TValue>* parent)
{
	if (oldnode == nullptr)
		return nullptr;

	Node<TKey, TValue>* newnode = new Node<TKey, TValue>(oldnode->key, oldnode->data, parent, oldnode->color);
	newnode->left = GetCopy(oldnode->left, newnode);
	newnode->right = GetCopy(oldnode->right, newnode);

	return newnode;
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::rotate_right(Node<TKey, TValue>* node)
{
	if (node == root)
		root = node->left;
	Node<TKey, TValue>* node_left = node->left;
	node->left = node_left->right;
	if (node->left)
		node->left->parent = node;
	if (node->parent && node == node->parent->left)
		node->parent->left = node_left;
	else if (node->parent && node == node->parent->right)
		node->parent->right = node_left;
	node_left->parent = node->parent;
	node_left->right = node;
	node->parent = node_left;
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::rotate_left(Node<TKey, TValue>* node)
{
	if (node == root)
		root = node->right;
	Node<TKey, TValue>* node_right = node->right;
	node->right = node_right->left;
	if (node->right)  
		node->right->parent = node;
	if (node->parent && node == node->parent->right)
		node->parent->right = node_right;
	else if (node->parent && node == node->parent->left)
		node->parent->left = node_right;
	node_right->parent = node->parent;
	node_right->left = node;
	node->parent = node_right;
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::BalanceAfterInsertion(Node<TKey, TValue>* node)
{
	while (node->parent != nullptr && node->parent->color == false)
	{
		if (node->parent == node->parent->parent->left)
		{
			Node<TKey, TValue>* uncle = node->parent->parent->right;
			if ((uncle? uncle->color : true) == false)
			{
				node->parent->color = true;
				uncle->color = true;
				node->parent->parent->color = false;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->right)
				{
					node = node->parent;
					rotate_left(node); 
				}
				node->parent->color = true;
				node->parent->parent->color = false;
				rotate_right(node->parent->parent); 
			}
		}
		else
		{
			Node<TKey, TValue>* uncle = node->parent->parent->left;
			if ((uncle? uncle->color : true) == false)
			{
				node->parent->color = true;
				uncle->color = true;
				node->parent->parent->color = false;
				node = node->parent->parent;
			}
			else
			{
				if (node == node->parent->left)
				{
					node = node->parent;
					rotate_right(node);
				}
				node->parent->color = true;
				node->parent->parent->color = false;
				rotate_left(node->parent->parent);
			}
		}
	}
	root->color = true;
}

template<typename TKey, typename TValue>
inline void RB_Tree<TKey, TValue>::BalanceAfterRemoval(Node<TKey, TValue>* node, Node<TKey, TValue>* _parent) 
{
	Node<TKey, TValue>* parent;
	while (node != root && ( node? node->color : true) == true)
	{ 
		parent = node ? node->parent : _parent;
		if (node == parent->left)
		{
			Node<TKey, TValue>* brother = parent->right;
			if (brother->color == false)
			{
				brother->color = true;
				parent->color = false;
				rotate_left(parent);
				brother = parent->right;
			}
			if ((brother->left? brother->left->color : true) == true && 
				(brother->right? brother->right->color : true) == true)
			{
				brother->color = false;
				node = parent;
			}
			else 
			{
				if ((brother->right? brother->right->color : true) == true)
				{
					brother->left->color = true;
					brother->color = false;
					rotate_right(brother);
					brother = parent->right;
				}
				brother->color = parent->color;
				parent->color = true;
				brother->right->color = true;
				rotate_left(parent);
				node = root;
			}
		}
		else 
		{
			Node<TKey, TValue>* brother = parent->left;
			if (brother->color == false)
			{
				brother->color = true;
				parent->color = false;
				rotate_right(parent);
				brother = parent->left;
			}
			if ((brother->left ? brother->left->color : true) == true &&
				(brother->right ? brother->right->color : true) == true)
			{
				brother->color = false;
				node = parent;
			}
			else
			{
				if ((brother->left ? brother->left->color : true) == true)
				{
					brother->right->color = true;
					brother->color = false;
					rotate_left(brother);
					brother = parent->left;
				}
				brother->color = parent->color;
				parent->color = true;
				brother->left->color = true;
				rotate_right(parent);
				node = root;
			}
		}
	}
	 if (node) node->color = true;
}