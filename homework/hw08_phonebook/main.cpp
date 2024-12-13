#ifndef __PROGTEST__
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

constexpr int PHONE_DIGITS = 10;

typedef struct TNode {
  char* m_Name;
  struct TNode* m_Child[PHONE_DIGITS];
} TNODE;

typedef struct {
  TNODE* m_Root;
  int m_Size;
} TPHONEBOOK;

#endif /* __PROGTEST__ */

typedef TPHONEBOOK Phonebook;
typedef TNODE Node;

Node* Node_malloc() {
  Node* node = (Node*)malloc(sizeof(Node));
  memset(node, 0, sizeof(Node));

  return node;
}

int Node_countChildren(Node* node) {
  int count = 0;
  for (int digit = 0; digit < 10; digit += 1) {
    if (node->m_Child[digit] == NULL) continue;
    count += 1;
  }

  return count;
}

void Node_setName(Node* node, const char* name) {
  if (node->m_Name != NULL) {
    free(node->m_Name);
  }

  node->m_Name = (char*)malloc(strlen(name) * sizeof(char) + 1);
  strcpy(node->m_Name, name);
}

void Node_unsetName(Node* node) {
  free(node->m_Name);
  node->m_Name = NULL;
}

void Node_free(Node* node) {
  if (node == NULL) return;

  if (node->m_Name != NULL) {
    Node_unsetName(node);
  }

  for (int digit = 0; digit < 10; digit += 1) {
    if (node->m_Child[digit] == NULL) continue;
    Node_free(node->m_Child[digit]);
  }

  free(node);
}

bool addPhone(Phonebook* book, const char* phone, const char* name) {
  Node* node = book->m_Root;
  Node** p_branch = NULL;

  if (book->m_Root == NULL) {
    book->m_Root = Node_malloc();
    node = book->m_Root;
    p_branch = &book->m_Root;
  }

  int phoneLength = strlen(phone);
  for (int i = 0; i < phoneLength; i += 1) {
    char digitChar = phone[i];
    int digit = digitChar - '0';

    if (!isdigit(digitChar)) {
      if (p_branch != NULL) {
        Node_free(*p_branch);
        *p_branch = NULL;
      }

      return false;
    }

    Node** p_child = &node->m_Child[digit];

    if (*p_child == NULL) {
      *p_child = Node_malloc();

      if (p_branch == NULL) {
        p_branch = p_child;
      }
    }

    node = *p_child;
  }

  if (node->m_Name == NULL) {
    book->m_Size += 1;
  }

  Node_setName(node, name);
  return true;
}

void delBook(Phonebook* book) {
  Node_free(book->m_Root);
}

bool delPhone(Phonebook* book, const char* phone) {
  if (book->m_Root == NULL) {
    return false;
  }

  Node** p_node = &book->m_Root;
  Node** p_branch = NULL;

  int phoneLength = strlen(phone);
  for (int i = 0; i < phoneLength; i += 1) {
    char digitChar = phone[i];
    int digit = digitChar - '0';

    if (!isdigit(digitChar)) {
      return false;
    }

    Node** p_child = &(*p_node)->m_Child[digit];

    if (*p_child == NULL) {
      return false;
    }

    int childrenCount = Node_countChildren(*p_node);

    bool isOnlyChild = childrenCount == 1;
    bool isBranchSet = p_branch != NULL;
    bool hasName = (*p_node)->m_Name != NULL;

    if (!isBranchSet && isOnlyChild && !hasName) {
      p_branch = p_node;
    } else if (!isOnlyChild || hasName) {
      p_branch = NULL;
    }

    p_node = p_child;
  }

  if ((*p_node)->m_Name == NULL) {
    return false;
  }

  Node_unsetName(*p_node);

  int childrenCount = Node_countChildren(*p_node);
  bool isLeafNode = childrenCount == 0;
  bool isBranchSet = p_branch != NULL;

  if (!isBranchSet && isLeafNode) {
    p_branch = p_node;
  } else if (!isLeafNode) {
    p_branch = NULL;
  }

  if (p_branch != NULL) {
    Node_free(*p_branch);
    *p_branch = NULL;
  }

  book->m_Size -= 1;
  return true;
}

const char* findPhone(Phonebook* book, const char* phone) {
  if (book->m_Root == NULL) return NULL;

  Node* node = book->m_Root;
  const char* name = NULL;

  int phoneLength = strlen(phone);
  for (int i = 0; i < phoneLength; i += 1) {
    char digitChar = phone[i];
    int digit = digitChar - '0';

    Node* child = node->m_Child[digit];

    if (child == NULL) break;

    if (child->m_Name != NULL) {
      name = child->m_Name;
    }

    node = child;
  }

  return name;
}

#ifndef __PROGTEST__
int main() {}
#endif /* __PROGTEST__ */
