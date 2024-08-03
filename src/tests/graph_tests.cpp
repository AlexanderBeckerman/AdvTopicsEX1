// #include "graph.h"
// #include <gtest/gtest.h>

// class GraphTest : public ::testing::Test {
//   protected:
//     Graph g;

//     void SetUp() override {
//         //     1
//         //    / \.
//         //   2   3
//         //    \ /
//         //     4
//         //      \.
//         //       5
//         g.addEdge(1, 2);
//         g.addEdge(1, 3);
//         g.addEdge(2, 4);
//         g.addEdge(3, 4);
//         g.addEdge(4, 5);
//     }
//     void TearDown() override {}
// };

// TEST_F(GraphTest, BFSTest) {
//     std::vector<int> path;
//     ASSERT_TRUE(g.bfs(1, 5, path));
//     ASSERT_EQ(path.size(), 4);
//     ASSERT_EQ(path[0], 1);
//     ASSERT_EQ(path[1], 2);
//     ASSERT_EQ(path[2], 4);
//     ASSERT_EQ(path[3], 5);
// }

// TEST_F(GraphTest, DFSTest) {
//     std::vector<int> path;
//     ASSERT_TRUE(g.dfs(1, 5, path));
//     ASSERT_EQ(path.size(), 4);
//     ASSERT_EQ(path[0], 1);
//     ASSERT_TRUE(path[1] == 2 || path[1] == 3);
//     ASSERT_EQ(path[2], 4);
//     ASSERT_EQ(path[3], 5);
// }
