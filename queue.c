#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head) {
        return NULL;
    }
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;
    }
    struct list_head *node, *safe;

    list_for_each_safe (node, safe, head) {
        element_t *entry = list_entry(node, element_t, list);

        list_del(node);

        if (entry->value) {
            free(entry->value);
        }
        free(entry);
    }
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element) {
        return false;
    }
    new_element->value = strdup(s);
    if (!new_element->value) {
        free(new_element);
        return false;
    }
    list_add(&new_element->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head) {
        return false;
    }
    element_t *new_element = malloc(sizeof(element_t));
    if (!new_element) {
        return false;
    }
    new_element->value = strdup(s);
    if (!new_element->value) {
        free(new_element);
        return false;
    }
    list_add_tail(&new_element->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    struct list_head *first = head->next;

    element_t *element = list_entry(first, element_t, list);

    if (sp != NULL && bufsize > 0) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(first);

    return element;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    struct list_head *last = head->prev;

    element_t *element = list_entry(last, element_t, list);

    if (sp != NULL && bufsize > 0) {
        strncpy(sp, element->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_del(last);

    return element;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head) {
        return 0;
    }

    int count = 0;

    struct list_head *current_element;

    current_element = head->next;
    while (current_element != head) {
        count += 1;
        current_element = current_element->next;
    }

    return count;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head || !head->next) {
        return 0;
    }
    struct list_head *slow = head->next, *fast = head->next;
    while (fast->next != head && fast->next->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }
    element_t *element = list_entry(slow, element_t, list);
    list_del(slow);

    q_release_element(element);
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head || list_empty(head))
        return false;

    struct list_head *current = head->next;

    while (current != head && current->next != head) {
        element_t *current_element = list_entry(current, element_t, list);
        struct list_head *compare = current->next;
        struct list_head *next_current = current->next;
        bool found_duplicate = false;

        while (compare != head) {
            struct list_head *next_compare = compare->next;
            element_t *compare_element = list_entry(compare, element_t, list);

            if (strcmp(current_element->value, compare_element->value) == 0) {
                found_duplicate = true;

                if (next_current == compare) {
                    next_current = next_compare;
                }

                list_del(compare);
                q_release_element(compare_element);
            }

            compare = next_compare;
        }

        if (found_duplicate) {
            list_del(current);
            q_release_element(current_element);
            current = next_current;
        } else {
            current = current->next;
        }
    }
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
