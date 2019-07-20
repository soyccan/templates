int longest_common_subsequence(int seq1[], int len1, int seq2[], int len2) {
  int** dp = new int*[len1+1]();
  // dp[i][j]: LCS length for seq1[0:i] and seq2[0:j]

  for (int i=1; i<=len1; i++) {
    dp[i] = new int[len2+1]();
    for (int j=1; j<=len2; j++) {
      if (seq1[i-1] == seq2[j-1]) {
        dp[i][j] = dp[i-1][j-1] + 1;
      } else {
        dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
      }
    }
  }

  return dp[len1][len2];
}