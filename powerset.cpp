unsigned pow2(unsigned exponent) {
  return 1 << exponent;
}

unsigned log2(unsigned arg) {
  if (arg == 1)
    return 0;
  else
    return log2(arg>>1) + 1;
}

/**
 * @brief  Enumerate all subsets of a set
 * @param superset  The set itself
 * @param result  An output iterator to write the subsets to
 * @return  The iterator next to the last subset written,
 *          i.e. "result + pow(2, superset.size())"
 */
template<typename Container, typename OutputIterator>
OutputIterator powerset(Container superset, OutputIterator result) {
  Container bt[pow2(superset.size()+1)]; // binary tree
  bt[1] = Container(); // root

  auto elem = superset.begin();
  for (unsigned i=2; i<pow2(superset.size()+1); i+=2) {
    bt[i] = bt[i+1] = bt[i/2]; // inherit from father
    bt[i+1].insert(*elem);
    if (pow2(log2(i+2)) == i+2) // i+2 == pow(2, N) where N is interger
      elem++;
  }

  // int level = 1; // root is level 0
  // for (auto elem : superset) {
  //   for (unsigned i=pow2(level); i<=pow2(level+1)-2; i+=2) {
  //     bt[i] = bt[i+1] = bt[i/2]; // inherit from father
  //     bt[i+1].insert(elem);
  //   }
  //   level++;
  // }

  return copy(bt + pow2(superset.size()),
              bt + pow2(superset.size()+1),
              result); // leaves
}