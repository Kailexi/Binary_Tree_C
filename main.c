#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


typedef struct Node {
    int value; // Значение узла
    struct Node* left; // Указатель на левое поддерево
    struct Node* right; // Указатель на правое поддерево
} Node;


Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // Выделяем память для нового узла
    newNode->value = value; // Присваиваем значение узлу
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode; // Возвращаем указатель на новый узел
}

// Функция для нахождения максимального узла в поддереве
Node* findMax(Node* root) {
    while (root->right != NULL) { // Проходим по правым узлам
        root = root->right;
    }
    return root; // Возвращаем максимальный узел
}

// Функция для удаления узла из дерева
Node* deleteNode(Node* root, int value, char* result) {
    if (root == NULL) {
        strcpy(result, "CANNOT"); // Узел не найден
        return NULL;
    }

    // Если найден узел для удаления
    if (root->value == value) {
        if (root->left == NULL) { // Если у узла нет левого потомка
            Node* temp = root->right; // Сохраняем правое поддерево
            free(root); // Освобождаем память узла
            strcpy(result, "DONE"); // Удаление выполнено
            return temp; // Возвращаем правое поддерево
        } else if (root->right == NULL) { // Если у узла нет правого потомка
            Node* temp = root->left; // Сохраняем левое поддерево
            free(root); // Освобождаем память узла
            strcpy(result, "DONE"); // Удаление выполнено
            return temp; // Возвращаем левое поддерево
        } else { // Если у узла есть два потомка
            Node* maxNode = findMax(root->left); // Находим максимальный узел в левом поддереве
            root->value = maxNode->value; // Переносим значение максимального узла
            root->left = deleteNode(root->left, maxNode->value, result); // Удаляем максимальный узел
            return root;
        }
    }

    // Если текущий узел не совпадает с искомым, продолжаем рекурсивный поиск
    return root;
}

// Функция для удаления дочернего узла с указанным значением из родительского узла
Node* delete(Node* root, int parentValue, int valueToDelete, char* result) {
    if (root == NULL) {
        strcpy(result, "CANNOT"); // Узел не найден
        return NULL;
    }

    // Если найден родительский узел, пробуем удалить дочерний
    if (root->value == parentValue) {
        if (root->left != NULL && root->left->value == valueToDelete) { // Если узел в левом поддереве
            root->left = deleteNode(root->left, valueToDelete, result); // Удаляем узел
            return root;
        } else if (root->right != NULL && root->right->value == valueToDelete) { // Если узел в правом поддереве
            root->right = deleteNode(root->right, valueToDelete, result); // Удаляем узел
            return root;
        } else {
            strcpy(result, "NO_CHILD"); // У родителя нет дочернего узла с таким значением
            return root;
        }
    }

    // Рекурсивно ищем родительский узел в поддеревьях
    root->left = delete(root->left, parentValue, valueToDelete, result);
    root->right = delete(root->right, parentValue, valueToDelete, result);
    return root;
}

// Функция для добавления нового узла в дерево
Node* add(Node* root, int value) {
    if (root == NULL) { // Если дерево пустое, создаем новый узел
        return createNode(value);
    }

    // Рекурсивно добавляем узел в левое или правое поддерево
    if (value < root->value) {
        root->left = add(root->left, value);
    } else if (value > root->value) {
        root->right = add(root->right, value);
    }

    return root; // Возвращаем корень дерева
}

// Функция для поиска значения в дереве
void search(Node* root, int value, char* result) {
    if (root == NULL) {
        strcpy(result, "NO"); // Если дерево пустое или значение не найдено
        return;
    }
    if (value == root->value) {
        strcpy(result, "YES"); // Значение найдено
    } else if (value < root->value) {
        search(root->left, value, result); // Рекурсивный поиск в левом поддереве
    } else {
        search(root->right, value, result); // Рекурсивный поиск в правом поддереве
    }
}

// Функция для печати дерева с отступами для визуального представления структуры
void printTreeIndented(Node* root, int depth, Node* parent) {
    if (root == NULL) {
        return;
    }

    // Рекурсивный обход левого поддерева
    printTreeIndented(root->left, depth + 1, root);

    // Печать текущего узла с отступами в зависимости от глубины
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    // Печать узла, указание на корень или родителя
    if (parent) {
        printf("|-- ");
        printf("%d (Parent: %d)\n", root->value, parent->value);
    } else {
        printf("|-- %d (Root)\n", root->value);
    }

    // Рекурсивный обход правого поддерева
    printTreeIndented(root->right, depth + 1, root);
}

// Функция для добавления дочернего узла к указанному родительскому узлу
void addChild(Node* root, int parentValue, int childValue, char* result) {
    if (root == NULL) {
        strcpy(result, "CANNOT"); // Если дерево пустое
        return;
    }
    if (root->value == parentValue) { // Если найден родитель
        if (root->left == NULL) {
            root->left = createNode(childValue); // Добавляем дочерний узел в левое поддерево
            strcpy(result, "DONE (LEFT)");
        } else if (root->right == NULL) {
            root->right = createNode(childValue); // Добавляем дочерний узел в правое поддерево
            strcpy(result, "DONE (RIGHT)");
        } else {
            strcpy(result, "FULL"); // У родителя уже есть два потомка
        }
        return;
    }
    // Рекурсивный поиск родителя в поддеревьях
    addChild(root->left, parentValue, childValue, result);
    if (strcmp(result, "") == 0) {
        addChild(root->right, parentValue, childValue, result);
    }
}

// Функция для отображения меню с доступными командами
void displayMenu() {
    printf("\nВыберите команду:\n");
    printf("1. ADD <value> - Добавить значение в дерево\n");
    printf("2. DELETE <parent> <value> - Удалить значение из дерева\n");
    printf("3. SEARCH <value> - Найти значение в дереве\n");
    printf("4. ADDCHILD <parent> <value> - Добавить дочерний узел\n");
    printf("5. PRINTTREE - Вывести дерево\n");
    printf("6. EXIT - Выйти из программы\n");
}

int main() {

    setlocale(LC_ALL, "Russian");

    Node* root = NULL; // Инициализация корня дерева
    char result[16]; // Переменная для хранения результатов команд
    int choice, parentValue, valueToDelete, searchValue;

    while (1) {
        displayMenu();
        printf("Введите номер команды: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Введите значение для добавления: ");
                scanf("%d", &valueToDelete);
                root = add(root, valueToDelete);
                break;
            case 2:
                printf("Введите родительский узел: ");
                scanf("%d", &parentValue);
                printf("Введите значение для удаления: ");
                scanf("%d", &valueToDelete);
                root = delete(root, parentValue, valueToDelete, result);
                printf("%s\n", result);
                break;
            case 3:
                printf("Введите значение для поиска: ");
                scanf("%d", &searchValue);
                search(root, searchValue, result);
                printf("%s\n", result);
                break;
            case 4:
                printf("Введите родительский узел: ");
                scanf("%d", &parentValue);
                printf("Введите дочерний узел: ");
                scanf("%d", &valueToDelete);
                addChild(root, parentValue, valueToDelete, result);
                printf("%s\n", result);
                break;
            case 5:
                printTreeIndented(root, 0, NULL);
                break;
            case 6:
                printf("Выход из программы.\n");
                return 0; // Выход из программы
            default:
                printf("Некорректная команда.\n");
        }
    }

    return 0;
}
