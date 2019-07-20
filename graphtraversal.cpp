template<typename Container, typename UnaryOperation>
void depth_first_search(const Container& adjacency_list, bool visited[],
                        UnaryOperation func, int start=0) {
  func(start);
  for (auto point : adjacency_list[start]) {
    if (not visited[point]) {
      visited[point] = true;
      depth_first_search(adjacency_list, visited, func, point);
    }
  }
}

template<typename Container, typename UnaryOperation>
void breadth_first_search(const Container& adjacency_list, bool visited[],
                          UnaryOperation func, int start=0) {
  queue<int> waiting;
  waiting.push(start);
  while (not waiting.empty()) {
    int current = waiting.front();
    waiting.pop();
    func(current);
    for (int point : adjacency_list[current]) {
      if (not visited[point]) {
        visited[point] = true;
        waiting.push(point);
      }
    }
  }
}