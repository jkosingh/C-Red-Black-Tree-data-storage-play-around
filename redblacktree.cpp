#ifdef _REDBLACKTREE_H_


// recursive helper function for deep copy
// creates a new node "thisnode" based on sourcenode's contents, links back to parentnode,
//   and recurses to create left and right children
template <class T>
Node<T>* RedBlackTree<T>::CopyTree(Node<T>* sourcenode, Node<T>* parentnode)
{
	//empty tree
	if (sourcenode == NULL)
	{
		return NULL;
	}

	else
	{
		//copy contents of sourcenode into our need tree
		Node<T> * newNode = new Node<T>(sourcenode->data);
		newNode->is_black = sourcenode->is_black;
		newNode->p = parentnode;

		//call left and right
		newNode->left = CopyTree(sourcenode->left, newNode);
		newNode->right = CopyTree(sourcenode->right, newNode);
		return newNode;
	}

}


// recursive helper function for tree deletion
// deallocates nodes in post-order
template <class T>
void RedBlackTree<T>::RemoveAll(Node<T>* node)
{
	if (node != NULL)
	{
		RemoveAll(node->left);
		RemoveAll(node->right);
		delete node;
	}
}



// Removal of an item from the tree.
// Must deallocate deleted node after RBDeleteFixUp returns
template <class T>
bool  RedBlackTree<T>::Remove(T item)
{
	if (Search(item) != true)
	{
		return false;
	}

	Node<T>* z = root;

	while (z != NULL)
	{
		if (item == z->data)
		{
			break; // item has been found, exit while loop
		}
		else if (item < z->data)
		{
			z = z->left;
		}
		else
		{
			z = z->right;
		}
	}

	Node<T>* y = NULL; // y is the one you want to replace z's data in node with and x is it's child node

	if (z->left == NULL || z->right == NULL) // if z has one or 0 chidren
	{
		y = z; //node to be removed
	}
	else
	{
		y = Predecessor(z); //Z has two children so need to replace with predecessor
	}
	Node<T>* x;


	if (y->left != NULL)
	{
		x = y->left; // child of the predecessor in case it has one
	}
	else
	{
		x = y->right;  // child of the predecessor in case it has one
	}

	if (x != NULL) // if child is there
	{
		x->p = y->p; //detach child from prdecessor (if x is not null)
	}

	//check if y is a left or right child
	bool lefty = false;
	if (y != NULL)// Node to be removed has predecessor   
	{
		if (y->p != NULL && y->p->left != NULL && y == y->p->left)
		{
			lefty = true;
		}
		else if (y->p != NULL && y->p->right != NULL && y == y->p->right)
		{
			lefty = false;
		}
	}


	if (y->p == NULL) //y is the root
	{
		root = x; // y doesn't have parent must be root
	}
	else
	{
		// Attach x to y’s parent
		if (y == y->p->left) //left child
		{
			y->p->left = x;
		}
		else
		{
			y->p->right = x;
		}
	}
	if (y != z) // exchange data and just delete the predcessor 
	{
		z->data = y->data; //exchange data
	}

	Node<T>* yParent = y->p;

	

	if (y->is_black == true && size > 1)
	{

		//delete node
		y->left = NULL;
		y->right = NULL;
		delete y;

		//if node to delete in black send it to fixup

		RBDeleteFixUp(x, yParent, lefty);
	}

	//red node does not need fixup
	else
	{
		//delete node
		y->left = NULL;
		y->right = NULL;
		delete y;
	}
	size--;
	return true;
}




// Tree fix, performed after removal of a black node
// Note that the parameter x may be NULL
template <class T>
void RedBlackTree<T>::RBDeleteFixUp(Node<T>* x, Node<T>* xparent, bool xisleftchild)
{
	Node<T>* y;
	while (x != root && (x == NULL || x->is_black == true))
	{
		if ( xisleftchild == true)// xisleftchild
		{
			y = xparent->right; //y is right sibling


			if (y->is_black == false) // sibling is red
			{
				y->is_black = true;
				xparent->is_black = false;
				LeftRotate(xparent);
				y = xparent->right;
			}
			if ((y->left == NULL || y->left->is_black == true) && (y->right == NULL || y->right->is_black == true))// both y's children are black
			{
				y->is_black = false;
				x = xparent;
				//xparent = xparent->p;
				if (xparent->p != NULL)
				{
					xparent = xparent->p;
					if (x == xparent->left)
					{
						xisleftchild = true;
					}
					else
					{
						xisleftchild = false;
					}
				}
			}
			else
			{
				if (y->right == NULL || y->right->is_black == true) // y has red child inline with parent
				{
					y->left->is_black = true; // change child and parent nodes colors the rotate and back into while loop
					y->is_black = false;
					RightRotate(y);
					y = xparent->right; 
				}
				y->is_black = xparent->is_black;
				xparent->is_black = true;
				y->right->is_black = true; //color inline child black
				LeftRotate(xparent);
				x = root;


			}
		}
		else // OTHER SIDE NOW 
		{
			y = xparent->left; // left sibling

			if (y->is_black == false) //IF SIBLING IS RED
			{
				y->is_black = true;
				xparent->is_black = false;
				RightRotate(xparent);
				y = xparent->left;
			}
			if ((y->right == NULL || y->right->is_black == true) && (y->left == NULL || y->left->is_black == true))//BOTH Y's CHILDREN BLACK
			{
				y->is_black = false;
				x = xparent;
				if (xparent->p != NULL)
				{
					xparent = xparent->p;
					if (x == xparent->left)
					{
						xisleftchild = true;
					}
					else
					{
						xisleftchild = false;
					}
				}
				
			}
			else// Y HAS RED CHILD INLINE WITH PARENT CASES
			{
				if (y->left == NULL || y->left->is_black == true) // y has red child inline with parent
				{
					y->right->is_black = true;
					y->is_black = false;
					LeftRotate(y);
					y = xparent->left; //GUESS
				}
					y->is_black = xparent->is_black;
					xparent->is_black = true;
		
					y->left->is_black = true; //color inline child black
					RightRotate(xparent);
					x = root;

			}
		}
	}

	x->is_black = true;
}


// Calculates the height of the tree
// Requires a traversal of the tree, O(n)
template <class T>
int RedBlackTree<T>::CalculateHeight(Node<T>* node) const
{
	if (node == NULL)
	{
		//empty tree so no height
		return 0;
	}
	// postorder traversal of tree
	int leftheight = CalculateHeight(node->left); 
	int rightheight = CalculateHeight(node->right);

	if (leftheight > rightheight)
	{
		return leftheight + 1;
	}
	else
	{
		return rightheight + 1;
	}
	return 0;
}

// default constructor
template <class T>
RedBlackTree<T>::RedBlackTree()
{
	root = NULL;
	size = 0;
}

// copy constructor, performs deep copy of parameter
template <class T>
RedBlackTree<T>::RedBlackTree(const RedBlackTree<T>& rbtree)
{
	if (rbtree.GetRoot() != NULL)
	{
		root = CopyTree(rbtree.GetRoot(), rbtree.GetRoot());
		size = rbtree.Size();
	}

	else // tree empty
	{
		root = NULL;
		size = 0;
	}

}

// destructor
// Must deallocate memory associated with all nodes in tree
template <class T>
RedBlackTree<T>::~RedBlackTree()
{
	RemoveAll(root);
	root = NULL;
	size = 0;
}
// overloaded assignment operator
template<class T>
RedBlackTree<T>& RedBlackTree<T>::operator=(const RedBlackTree<T>& rbtree)
{
	if (this != &rbtree)
	{
		RemoveAll(root); // right side

		if (rbtree.GetRoot() != NULL)
		{
			root = CopyTree(rbtree.GetRoot(), rbtree.GetRoot()->p);
			size = rbtree.Size();
		}

		else // tree empty
		{
			root = NULL;
			size = 0;
		}
	}

	return *this;

}





// Calls BSTInsert and then performs any necessary tree fixing.
// If item already exists, do not insert and return false.
// Otherwise, insert, increment size, and return true.
template<class T>
bool RedBlackTree<T>::Insert(T item)
{
	Node<T>* cursor;
	Node<T>* uncle;
	//look if item is already in the tree
	if (Search(item) == true)
	{
		return false;
	}

	//insert node normally as BST
	cursor = BSTInsert(item);
	size++;
	if (size == 1)
	{
		cursor = root;
		root->is_black = true;
		return true;
	}
	//if (size == 3)
	//{
	//	if (cursor->data > root->data && cursor->data == root->right->data)
	//	{
	//		//SET UP IN TREE SHAPE SO FIXED Cursor greater than root so placed on right
	//		root->is_black = true;
	//		root->left->is_black = false;
	//		root->right->is_black = false;
	//		return true;
	//	}
	//	else if (cursor->data < root->data && cursor->data == root->left->data)
	//	{
	//		//SET UP IN TREE SHAPE SO FIXED Cursor less than than root so placed on left
	//		root->is_black = true;
	//		root->left->is_black = false;
	//		root->right->is_black = false;
	//		return true;
	//	}
	//	if (cursor->p->p != NULL && cursor->p->p->left != NULL && cursor->p->p->left->left != NULL)
	//	{
	//		RightRotate(root); // Tree is straight left line at this point
	//	}
	//	else if (cursor->p->p != NULL && cursor->p->p->right != NULL && cursor->p->p->right->right != NULL)
	//	{
	//		LeftRotate(root); // Tree is straight right line at this point
	//	}
	//	else if (cursor->p->p->left != NULL && cursor->p->p->left->right != NULL)
	//	{
	//		LeftRotate(cursor->p); // CASE WHERE CURSOR IS GREATER THAN PARENT BUT PARENT IS LESS THAN ROOT
	//		RightRotate(root);
	//	}
	//	else if (cursor->p->p->right != NULL && cursor->p->p->right->left != NULL)
	//	{
	//		RightRotate(cursor->p);// CASE WHERE CURSOR IS LESS THAN PARENT BUT PARENT IS GREATER THAN ROOT
	//		LeftRotate(root);
	//	}

	//	root->is_black = true;
	//	root->left->is_black = false;
	//	root->right->is_black = false;
	//	return true;
	//}

	////Fix the colours of the nodes to t make it a RBT
	if (size >= 3) //fixes BST tree to become RBT
	{

		while (cursor != root && cursor->p->is_black == false)
		{
			//case where the parent is left child
			if (cursor->p->p && cursor->p == cursor->p->p->left)
			{
				//if the parent and uncle are both red fix the colours to black and granddad to red
				uncle = cursor->p->p->right;
				if (uncle != NULL && uncle->is_black == false)
				{
					cursor->p->is_black = true;
					uncle->is_black = true;
					cursor->p->p->is_black = false;
					cursor = cursor->p->p;
				}

				//uncle and parent have different colours
				else
				{
					//if inserted node is right child
					if (cursor == cursor->p->right)
					{
						cursor = cursor->p;
						LeftRotate(cursor);
						cursor->p->is_black = true;
						cursor->p->p->is_black = false;
						RightRotate(cursor->p->p);
					}

					//if inseted node is left child
					else
					{
						cursor->p->is_black = true;
						cursor->p->p->is_black = false;
						RightRotate(cursor->p->p);

					}

				}
			}

			//case where the parent is right child
			else if (cursor->p->p && cursor->p == cursor->p->p->right)
			{
				//if the parent and uncle are both red fix the colours to black and granddad to red
				uncle = cursor->p->p->left;
				if (uncle != NULL && uncle->is_black == false)
				{
					cursor->p->is_black = true;
					uncle->is_black = true;
					cursor->p->p->is_black = false;
					cursor = cursor->p->p;
				}

				//uncle and parent have different colours
				else
				{
					//if inserted node is right child
					if (cursor == cursor->p->right)
					{
						cursor->p->is_black = true;
						cursor->p->p->is_black = false;
						LeftRotate(cursor->p->p);
					}

					//if inserted node is left child
					else
					{
						cursor = cursor->p;
						RightRotate(cursor);
						cursor->p->is_black = true;
						cursor->p->p->is_black = false;
						LeftRotate(cursor->p->p);

					}

				}

			}
		}

	}//end of if for size>3

	 //make sure root is black


	root->is_black = true;

	return true;

}

// deletes all nodes in the tree. Calls recursive helper function.
template <class T>
void RedBlackTree<T>::RemoveAll()
{
	RemoveAll(root); // start from root postorder traversal
	root = NULL;
	size = 0;
}

// returns the number of items in the tree
template<class T>
int  RedBlackTree<T>::Size() const
{
	return size;
}

template<class T>
int RedBlackTree<T>::Height() const
{
	return CalculateHeight(root);
}




#endif 