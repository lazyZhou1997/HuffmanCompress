#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <iostream>
#include <string>

//测试者：周秦春 通过
//作者：李东晋
//日期：2016/11/24/21:10
//实现Huffman树
using namespace std;

//根节点
template <typename E>
class HuffNode {
public:
    virtual ~HuffNode() {}
    virtual int weight() = 0;
    virtual bool isLeaf() = 0;
};

//叶子节点
template <typename E>
class LeafNode : public HuffNode<E> 
{
private:
    E it;
    int wgt;
public:
	
    LeafNode(const E& val, int freq) 
	{
        it = val;
        wgt = freq;
    }

	//返回叶子节点的值
    E val() 
	{
        return it;
    }

	//返回叶子节点的权值
    int weight() 
	{
        return wgt;
    }

	//判断是否是叶子节点
    bool isLeaf() 
	{
        return true;
    }
};

//节点
template <typename E>
class IntlNode : public HuffNode<E> {
private:
    HuffNode<E>* lc;
    HuffNode<E>* rc;
    int wgt;
public:
    IntlNode(HuffNode<E>* l, HuffNode <E>* r) 
	{
        wgt = l->weight() + r->weight();
        lc = l;
        rc = r;
    }

	//返回内部节点的权值
    int weight() 
	{
        return wgt;
    }

	//设置内部节点的权值
	void setWeight(int weight)
	{
		wgt = weight;
	}
    
	//返回左子树
	HuffNode<E>* left() const 
	{
        return lc;
    }

	//设置左子树
    void setLeft(HuffNode<E>* b) 
	{
        lc = (HuffNode<E>*)b;
    }

	//返回右子树
    HuffNode<E>* right() const 
	{
        return rc;
    }

	//设置右子树
    void setRight(HuffNode<E>* b) 
	{
        rc = (HuffNode<E>*)b;
    }

	//判断是否是叶子节点
    bool isLeaf() 
	{
        return false;
    }
};

//Huffman树
template <typename E>
class HuffTree {
private:
    HuffNode<E>* Root;
public:
    HuffTree() {}

	//叶节点构造Huffman树
    HuffTree(E& val, int freq) 
	{
        Root = new LeafNode<E>(val, freq);
    }
	//通过Huffman树构造Huffman树
    HuffTree(HuffTree<E>* l, HuffTree<E>* r) {
        Root = new IntlNode<E>(l->root(), r->root());
    }

	//释放空间
    ~HuffTree() {}

	//返回根节点
    HuffNode<E>* root() 
	{
        return Root;
    }
	
	//返回根节点的权值
    int weight() 
	{
        return Root->weight();
    }


    //遍历Huffman树
    void traverse(HuffNode<E>* root) 
	{
        if (root == NULL) return;

        if (root->isLeaf())
            cout << "Leaf:" << root->weight() <<((LeafNode<E>*)root)->val()<< endl;
        else {
            IntlNode<E>* rt = (IntlNode<E>*)root;
            traverse(rt->left());
            cout << "Internal:" << rt->weight() << endl;
            traverse(rt->right());
        }
    }


};

//构建Huffman树
template <typename E>
HuffTree<E>* buildTree(HuffTree<E>** TreeArray, int count) {
    heap<E>* forest = new heap<E>(TreeArray, count, count);

	/*LeafNode<char> *ch = (LeafNode<char>*)(forest->removefirst());
	for (int i = 0; i < count; i++)
	{
		if (NULL!=ch)
		{
			cout <<ch->val()<< endl;
		}
		
	}*/


    HuffTree<E> *temp1, *temp2, *temp3 = NULL;
    while (forest->size() > 1) 
	{
        temp1 = forest->removefirst();
        temp2 = forest->removefirst();
        temp3 = new HuffTree<E>(temp1, temp2);
		
		//temp3->traverse(temp3->root());
        
		forest->insert(temp3);
        delete temp1;
        delete temp2;
    }
    return temp3;
}

//小顶堆的实现
template<typename E>
class heap {
private:
    HuffTree<E>** Heap; //Pointer to the heap array;
    int maxsize; //Maximum size of the heap;
    int n; //Number of elements now in the heap;

           //function to put element in its correct place;
    void siftdown(int pos) 
	{
        while (!isLeaf(pos)) {
            int j = leftchild(pos);
            int rc = rightchild(pos);
            if ((rc < n) && (Heap[j]->weight() > Heap[rc]->weight()))
                j = rc;
            if (Heap[j]->weight() > Heap[pos]->weight())
                return;
            swap(Heap, pos, j);
            pos = j;
        }
    }
public:
    heap(HuffTree<E>** h, int max, int num) {
        Heap = h;
        maxsize = max;
        n = num;
        buildHeap();
    }
    int size() const {
        return n;
    }
    bool isLeaf(int pos) const {
        return (pos >= n / 2) && (pos < n);
    }
    int leftchild(int pos) const {
        return 2 * pos + 1;
    }
    int rightchild(int pos) const {
        return 2 * pos + 2;
    }
    int parent(int pos) const {
        return (pos - 1) / 2;
    }
    void buildHeap() {
        for (int i = n / 2; i >= 0; i--) {
            siftdown(i);
        }
    }

    void insert(HuffTree<E>* it) {
        //Assert(n < maxsize, "Heap is full");
        int curr = n++;
        Heap[curr] = it;

        while ((curr != 0) && (Heap[parent(curr)]->weight() > Heap[curr]->weight())) {
            swap(Heap, curr, parent(curr));
            curr = parent(curr);
        }
    }

    //remove first value
    HuffTree<E>* removefirst() {
        //Assert(n > 0, "Heap is empty");
        swap(Heap, 0, --n);
        if (n != 0)
            siftdown(0);
        return Heap[n];
    }

    //remove and return element at specified position
    HuffTree<E>* remove(int pos) {
        Assert((pos >= 0) && (pos < n), "Bad position");
        if (pos == (n - 1))
            n--;
        else {
            swap(Heap, pos, --n);
            while ((pos != 0) && (Heap[parent(pos)]->weight() > Heap[pos]->weight())) {
                swap(Heap, pos, parent[pos]);
                pos = parent(pos);
            }
            if (n != 0)
                siftdown(pos);
        }
        return Heap[n];
    }

    void swap(HuffTree<E>** array, int first, int second) {
        HuffTree<E>* temp = array[first];
        array[first] = array[second];
        array[second] = temp;
    }
};

#endif // HUFFMAN_H
