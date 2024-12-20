#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


typedef struct Node {
    int value; // �������� ����
    struct Node* left; // ��������� �� ����� ���������
    struct Node* right; // ��������� �� ������ ���������
} Node;


Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // �������� ������ ��� ������ ����
    newNode->value = value; // ����������� �������� ����
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode; // ���������� ��������� �� ����� ����
}

// ������� ��� ���������� ������������� ���� � ���������
Node* findMax(Node* root) {
    while (root->right != NULL) { // �������� �� ������ �����
        root = root->right;
    }
    return root; // ���������� ������������ ����
}

// ������� ��� �������� ���� �� ������
Node* deleteNode(Node* root, int value, char* result) {
    if (root == NULL) {
        strcpy(result, "CANNOT"); // ���� �� ������
        return NULL;
    }

    // ���� ������ ���� ��� ��������
    if (root->value == value) {
        if (root->left == NULL) { // ���� � ���� ��� ������ �������
            Node* temp = root->right; // ��������� ������ ���������
            free(root); // ����������� ������ ����
            strcpy(result, "DONE"); // �������� ���������
            return temp; // ���������� ������ ���������
        } else if (root->right == NULL) { // ���� � ���� ��� ������� �������
            Node* temp = root->left; // ��������� ����� ���������
            free(root); // ����������� ������ ����
            strcpy(result, "DONE"); // �������� ���������
            return temp; // ���������� ����� ���������
        } else { // ���� � ���� ���� ��� �������
            Node* maxNode = findMax(root->left); // ������� ������������ ���� � ����� ���������
            root->value = maxNode->value; // ��������� �������� ������������� ����
            root->left = deleteNode(root->left, maxNode->value, result); // ������� ������������ ����
            return root;
        }
    }

    // ���� ������� ���� �� ��������� � �������, ���������� ����������� �����
    return root;
}

// ������� ��� �������� ��������� ���� � ��������� ��������� �� ������������� ����
Node* delete(Node* root, int parentValue, int valueToDelete, char* result) {
    if (root == NULL) {
        strcpy(result, "CANNOT"); // ���� �� ������
        return NULL;
    }

    // ���� ������ ������������ ����, ������� ������� ��������
    if (root->value == parentValue) {
        if (root->left != NULL && root->left->value == valueToDelete) { // ���� ���� � ����� ���������
            root->left = deleteNode(root->left, valueToDelete, result); // ������� ����
            return root;
        } else if (root->right != NULL && root->right->value == valueToDelete) { // ���� ���� � ������ ���������
            root->right = deleteNode(root->right, valueToDelete, result); // ������� ����
            return root;
        } else {
            strcpy(result, "NO_CHILD"); // � �������� ��� ��������� ���� � ����� ���������
            return root;
        }
    }

    // ���������� ���� ������������ ���� � �����������
    root->left = delete(root->left, parentValue, valueToDelete, result);
    root->right = delete(root->right, parentValue, valueToDelete, result);
    return root;
}

// ������� ��� ���������� ������ ���� � ������
Node* add(Node* root, int value) {
    if (root == NULL) { // ���� ������ ������, ������� ����� ����
        return createNode(value);
    }

    // ���������� ��������� ���� � ����� ��� ������ ���������
    if (value < root->value) {
        root->left = add(root->left, value);
    } else if (value > root->value) {
        root->right = add(root->right, value);
    }

    return root; // ���������� ������ ������
}

// ������� ��� ������ �������� � ������
void search(Node* root, int value, char* result) {
    if (root == NULL) {
        strcpy(result, "NO"); // ���� ������ ������ ��� �������� �� �������
        return;
    }
    if (value == root->value) {
        strcpy(result, "YES"); // �������� �������
    } else if (value < root->value) {
        search(root->left, value, result); // ����������� ����� � ����� ���������
    } else {
        search(root->right, value, result); // ����������� ����� � ������ ���������
    }
}

// ������� ��� ������ ������ � ��������� ��� ����������� ������������� ���������
void printTreeIndented(Node* root, int depth, Node* parent) {
    if (root == NULL) {
        return;
    }

    // ����������� ����� ������ ���������
    printTreeIndented(root->left, depth + 1, root);

    // ������ �������� ���� � ��������� � ����������� �� �������
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // ������ ����, �������� �� ������ ��� ��������
    if (parent) {
        printf("|-- ");
        printf("%d (Parent: %d)\n", root->value, parent->value);
    } else {
        printf("|-- %d (Root)\n", root->value);
    }

    // ����������� ����� ������� ���������
    printTreeIndented(root->right, depth + 1, root);
}

// ������� ��� ���������� ��������� ���� � ���������� ������������� ����
void addChild(Node* root, int parentValue, int childValue, char* result) {
    if (root == NULL) {
        strcpy(result, "CANNOT"); // ���� ������ ������
        return;
    }
    if (root->value == parentValue) { // ���� ������ ��������
        if (root->left == NULL) {
            root->left = createNode(childValue); // ��������� �������� ���� � ����� ���������
            strcpy(result, "DONE (LEFT)");
        } else if (root->right == NULL) {
            root->right = createNode(childValue); // ��������� �������� ���� � ������ ���������
            strcpy(result, "DONE (RIGHT)");
        } else {
            strcpy(result, "FULL"); // � �������� ��� ���� ��� �������
        }
        return;
    }
    // ����������� ����� �������� � �����������
    addChild(root->left, parentValue, childValue, result);
    if (strcmp(result, "") == 0) {
        addChild(root->right, parentValue, childValue, result);
    }
}

// ������� ��� ����������� ���� � ���������� ���������
void displayMenu() {
    printf("\n�������� �������:\n");
    printf("1. ADD <value> - �������� �������� � ������\n");
    printf("2. DELETE <parent> <value> - ������� �������� �� ������\n");
    printf("3. SEARCH <value> - ����� �������� � ������\n");
    printf("4. ADDCHILD <parent> <value> - �������� �������� ����\n");
    printf("5. PRINTTREE - ������� ������\n");
    printf("6. EXIT - ����� �� ���������\n");
}

int main() {

    setlocale(LC_ALL, "Russian");

    Node* root = NULL; // ������������� ����� ������
    char result[16]; // ���������� ��� �������� ����������� ������
    int choice, parentValue, valueToDelete, searchValue;

    while (1) {
        displayMenu();
        printf("������� ����� �������: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("������� �������� ��� ����������: ");
                scanf("%d", &valueToDelete);
                root = add(root, valueToDelete);
                break;
            case 2:
                printf("������� ������������ ����: ");
                scanf("%d", &parentValue);
                printf("������� �������� ��� ��������: ");
                scanf("%d", &valueToDelete);
                root = delete(root, parentValue, valueToDelete, result);
                printf("%s\n", result);
                break;
            case 3:
                printf("������� �������� ��� ������: ");
                scanf("%d", &searchValue);
                search(root, searchValue, result);
                printf("%s\n", result);
                break;
            case 4:
                printf("������� ������������ ����: ");
                scanf("%d", &parentValue);
                printf("������� �������� ����: ");
                scanf("%d", &valueToDelete);
                addChild(root, parentValue, valueToDelete, result);
                printf("%s\n", result);
                break;
            case 5:
                printTreeIndented(root, 0, NULL);
                break;
            case 6:
                printf("����� �� ���������.\n");
                return 0; // ����� �� ���������
            default:
                printf("������������ �������.\n");
        }
    }

    return 0;
}
