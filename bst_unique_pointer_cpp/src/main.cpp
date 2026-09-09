#include "bst.h"

int main()
{
    BinarySearchTree<int> bst;
    bst.Insert(5);

    std::cout << "Erase(5)" << std::endl;
    bst.Erase(5);

    std::cout << "Destruction" << std::endl;
}