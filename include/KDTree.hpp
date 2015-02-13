/*
 * KDTree.hpp
 *
 *  Created on: Feb 12, 2015
 *      Author: tyler
 */

#ifndef KDTREE_HPP_
#define KDTREE_HPP_

#include <vector>

#define NULL 0

namespace MyEngine {

/**
 * K = dimentions
 * Data = Data type
 * Num = data type for comparisons. (Default float (1D))
 */
template <uint K, class Data, typename Num = float>
class KDTree {

public:

	/**
	 * This is a function that retrieves the nth dimension from the
	 * data type used in the KDTree. Values are 0 based.
	 */
	typedef std::function<Num(const Data const *, const uint)> Get;

private:

	KDTree* left = NULL;
	KDTree* right = NULL;
	Data* data = NULL;
	Get* get;

	KDTree(std::vector<Data*>& list, Get* get, uint depth);

public:

	KDTree(std::vector<Data*>& list, Get get) : get(&get){
		KDTree(list, &get, 0);
	}

	//This will rebuild the tree and balance it with any new nodes.
	void balance();

	/**
	 * This inserts a node into the tree. But does not do
	 * so in a balanced manner.
	 */
	void insert(Data*);

	/**
	 * Inverse of insert.
	 */
	void remove(Data*);

	//Call this if you want the data in the nodes to be deleted.
	void cleanUp();



};

}


#endif /* KDTREE_HPP_ */
