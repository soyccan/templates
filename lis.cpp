int longest_increasing_subsequence(int seq[], int len) {
  int dp[len];
  // dp[i]: LIS length for seq[0:i]

  for (int i=0; i<len; i++) {
    dp[i] = 1;
    for (int j=0; j<i; j++) {
      if (seq[j] <= seq[i]) {
        dp[i] = max(dp[i], dp[j]+1);
      }
    }
  }
  
  return dp[len-1];
}

int longest_increasing_subsequence(int seq[], int len) {
  vector<int> buf {seq[0]};

  for (int i=1; i<len; i++) {
    if (seq[i] > buf.back())
      buf.push_back(seq[i]);
    else
      *lower_bound(buf.begin(), buf.end(), seq[i]) = seq[i];
  }

  return buf.size();
}