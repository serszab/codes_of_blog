#include <bst.h>

#include <gtest/gtest.h>

#include <algorithm>

TEST(BST, DefaultConstructedIsEmpty)
{
    BinarySearchTree<int> bst;
    EXPECT_TRUE(bst.Empty());
}

TEST(BST, AfterInsertionNotEmpty)
{
    BinarySearchTree<int> bst;
    bst.Insert(42);
    EXPECT_FALSE(bst.Empty());
}

TEST(BST, SecondInsertionReturnsFalse)
{
    BinarySearchTree<int> bst;
    EXPECT_TRUE(bst.Insert(42));
    EXPECT_FALSE(bst.Insert(42));
}

TEST(BST, EmptyCannotContains) 
{
    BinarySearchTree<int> bst;
    EXPECT_FALSE(bst.Contains(42));
}

TEST(BST, Contains)
{
    BinarySearchTree<int> bst;
    for (const int item : { 42, 5, -3, 2 }) {
        bst.Insert(item);
        EXPECT_TRUE(bst.Contains(item));
    }
}

TEST(BST, EmptyHasNoMinAndMax)
{
    BinarySearchTree<int> bst;
    EXPECT_EQ(bst.Min(), std::nullopt);
    EXPECT_EQ(bst.Max(), std::nullopt);
}

TEST(BST, MinAndMax)
{
    BinarySearchTree<int> bst;
    for (const int item : { 42, 5, 50, -3, 2 }) {
        bst.Insert(item);
    }
    auto minValue = bst.Min();
    ASSERT_TRUE(minValue.has_value());
    EXPECT_EQ(minValue.value(), -3);

    auto maxValue = bst.Max();
    ASSERT_TRUE(maxValue.has_value());
    EXPECT_EQ(maxValue.value(), 50);
}

TEST(BST, InOrderTraversal)
{
    BinarySearchTree<int> bst;
    std::vector<int> items{ 5, 3, 7, 1, 8, 9, 2, 4, 6 };
    for (const int item : items) {
        bst.Insert(item);
    }

    std::vector<int> traversalResult;
    bst.Traverse<Traversal::InOrder>([&traversalResult](const int item){ traversalResult.push_back(item); });

    std::sort(items.begin(), items.end());
    ASSERT_EQ(traversalResult.size(), items.size());
    for (size_t i = 0; i < items.size(); ++i) {
        EXPECT_EQ(traversalResult[i], items[i]);
    }
}

TEST(BST, CopyConstructorAndPreOrderTraversal)
{
    BinarySearchTree<int> originalTree;
    std::vector<int> items{ 5, 3, 7, 1, 8, 9, 2, 4, 6 };
    for (const int item : items) {
        originalTree.Insert(item);
    }

    BinarySearchTree<int> copiedTree{ originalTree };

    std::vector<int> originalPreOrderResult;
    originalTree.Traverse<Traversal::PreOrder>([&originalPreOrderResult](const int item){ originalPreOrderResult.push_back(item); });
    std::vector<int> copiedPreOrderResult;
    copiedTree.Traverse<Traversal::PreOrder>([&copiedPreOrderResult](const int item){ copiedPreOrderResult.push_back(item); });
    ASSERT_EQ(originalPreOrderResult.size(), copiedPreOrderResult.size());
    for (size_t i = 0; i < originalPreOrderResult.size(); ++i) {
        EXPECT_EQ(originalPreOrderResult[i], copiedPreOrderResult[i]);
    }
}

TEST(BST, CopyAssignmentAndPreOrderTraversal)
{
    BinarySearchTree<int> originalTree;
    std::vector<int> items{ 5, 3, 7, 1, 8, 9, 2, 4, 6 };
    for (const int item : items) {
        originalTree.Insert(item);
    }

    BinarySearchTree<int> copiedTree;
    for (const int item : { 42, 3, 5, 8, 2, 0 }){
        copiedTree.Insert(item);
    }

    copiedTree = originalTree;

    std::vector<int> originalPreOrderResult;
    originalTree.Traverse<Traversal::PreOrder>([&originalPreOrderResult](const int item){ originalPreOrderResult.push_back(item); });
    std::vector<int> copiedPreOrderResult;
    copiedTree.Traverse<Traversal::PreOrder>([&copiedPreOrderResult](const int item){ copiedPreOrderResult.push_back(item); });
    ASSERT_EQ(originalPreOrderResult.size(), copiedPreOrderResult.size());
    for (size_t i = 0; i < originalPreOrderResult.size(); ++i) {
        EXPECT_EQ(originalPreOrderResult[i], copiedPreOrderResult[i]);
    }
}

TEST(BST, MoveConstructorAndInOrderTraversal)
{
    BinarySearchTree<int> originalTree;
    std::vector<int> items{ 5, 3, 7, 1, 8, 9, 2, 4, 6 };
    for (const int item : items) {
        originalTree.Insert(item);
    }

    BinarySearchTree<int> movedTree{ std::move(originalTree) };
    EXPECT_TRUE(originalTree.Empty());
    EXPECT_FALSE(movedTree.Empty());

    std::sort(items.begin(), items.end());
    std::vector<int> inOrderResult;
    movedTree.Traverse<Traversal::InOrder>([&inOrderResult](const int item){ inOrderResult.push_back(item); });
    ASSERT_EQ(inOrderResult.size(), items.size());
    for (size_t i = 0; i < inOrderResult.size(); ++i) {
        EXPECT_EQ(inOrderResult[i], items[i]);
    }
}

TEST(BST, MoveAssignmentAndInOrderTraversal)
{
    BinarySearchTree<int> originalTree;
    std::vector<int> items{ 5, 3, 7, 1, 8, 9, 2, 4, 6 };
    for (const int item : items) {
        originalTree.Insert(item);
    }

    BinarySearchTree<int> movedTree{ originalTree };
    movedTree.Insert(42);

    movedTree = std::move(originalTree);
    EXPECT_TRUE(originalTree.Empty());
    EXPECT_FALSE(movedTree.Empty());

    std::sort(items.begin(), items.end());
    std::vector<int> inOrderResult;
    movedTree.Traverse<Traversal::InOrder>([&inOrderResult](const int item){ inOrderResult.push_back(item); });
    ASSERT_EQ(inOrderResult.size(), items.size());
    for (size_t i = 0; i < inOrderResult.size(); ++i) {
        EXPECT_EQ(inOrderResult[i], items[i]);
    }
}

TEST(BST, HeightOfEmptyTree)
{
    BinarySearchTree<int> bst;
    EXPECT_EQ(bst.Height(), 0);
}

TEST(BST, HeightOfNonEmptyTree)
{
    BinarySearchTree<int> bst;
    for (const int item : { 5, 3, 7, 1, 8, 9, 2, 4, 6 }) {
        bst.Insert(item);
    }
    EXPECT_EQ(bst.Height(), 4);
}

TEST(BST, Erase)
{
    BinarySearchTree<int> bst;
    for (const int item : { 5, 3, 7, 1, 8, 9, 2, 4, 6 }) {
        bst.Insert(item);
    }

    EXPECT_FALSE(bst.Erase(42));

    EXPECT_TRUE(bst.Erase(2));
    EXPECT_FALSE(bst.Contains(2));

    EXPECT_TRUE(bst.Erase(8));
    EXPECT_FALSE(bst.Contains(8));
    EXPECT_TRUE(bst.Contains(9));
    EXPECT_EQ(bst.Height(), 3);

    EXPECT_TRUE(bst.Erase(5));
    EXPECT_FALSE(bst.Contains(5));
}