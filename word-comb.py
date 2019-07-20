# a program to try out word combinations in a shuffle string

import string, collections, sys

class TrieNode:
    def __init__(self):
        self.nxt = {}

def add(node, s, idx):
    if idx == len(s) or s[idx] == '\n':
        return
    if s[idx] not in node.nxt:
        node.nxt[s[idx]] = TrieNode()
    add(node.nxt[s[idx]], s, idx+1)

def dfs(node, words):
    # print()
    # print(node.nxt.keys())
    # print(charset)
    # print(sum(charset.values()))

    wordend = False
    if not node.nxt:
        wordend = True
        if sum(charset.values()) <= 0:
            print('bingo', ''.join(sol))
            return

        print(words)
        words.append(''.join(sol[ len(''.join(words)) : ]))
        if len(words) >= 4:
            return
        node = trie
        sys.stdout.write(''.join(sol) + '\n')

    for c in string.ascii_lowercase:
        if charset[c] > 0 and c in node.nxt:
            sol.append(c)
            charset[c] -= 1

            if dfs(node.nxt[c], words):
                return True

            sol.pop()
            charset[c] += 1

    if wordend:
        words.pop()

trie = TrieNode()
sol = []
words = []

charset = collections.Counter('ttcusithid_coy_ti_o}sfplnirct{_fsfi')
for c in 'tfcis' '____{}' '' 'difficult' 'you' '':
    charset[c] -= 1

for word in open('a.txt'):
    add(trie, word, 0)

dfs(trie, words)
print(''.join(sol))
print(words)
