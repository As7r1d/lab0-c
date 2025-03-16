#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

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
    if (q_size(head) <= 1) {
        return;
    }
    struct list_head *current = head->next;
    while (current != head && current->next != head) {
        struct list_head *first = current;
        struct list_head *second = current->next;

        list_del(second);
        list_add(second, first->prev);

        current = first->next;
    }
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }

    struct list_head *current = head->next;
    struct list_head *temp;

    head->next = head->prev;
    head->prev = current;

    while (current != head) {
        temp = current->next;

        current->next = current->prev;
        current->prev = temp;

        current = temp;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || k <= 1)
        return;

    int length = q_size(head);

    if (k > length)
        return;

    LIST_HEAD(result);

    while (length >= k) {
        LIST_HEAD(temp);

        struct list_head *kth_node = head->next;
        for (int i = 1; i < k; i++) {
            kth_node = kth_node->next;
        }

        list_cut_position(&temp, head, kth_node);

        q_reverse(&temp);

        list_splice_tail(&temp, &result);

        length -= k;
    }

    if (!list_empty(head)) {
        list_splice_tail(head, &result);
    }

    INIT_LIST_HEAD(head);
    list_splice(&result, head);


    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head left;
    struct list_head *fast, *slow;

    INIT_LIST_HEAD(&left);
    slow = head->next;
    fast = head->next->next;

    while (fast != head && fast->next != head) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // struct list_head *after_mid = slow->next;
    list_cut_position(&left, head, slow);

    q_sort(&left, descend);
    q_sort(head, descend);

    LIST_HEAD(result);

    while (!list_empty(&left) && !list_empty(head)) {
        element_t *left_elem = list_entry(left.next, element_t, list);
        element_t *right_elem = list_entry(head->next, element_t, list);

        int cmp = strcmp(left_elem->value, right_elem->value);
        struct list_head *chosen;


        if ((descend && cmp >= 0) || (!descend && cmp <= 0)) {
            chosen = left.next;
            list_del(chosen);
        } else {
            chosen = head->next;
            list_del(chosen);
        }

        list_add_tail(chosen, &result);
    }

    if (!list_empty(&left)) {
        list_splice_tail(&left, &result);
    } else if (!list_empty(head)) {
        list_splice_tail(head, &result);
    }

    INIT_LIST_HEAD(head);
    list_splice(&result, head);
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    if (!head || list_empty(head) || head->next->next == head) {
        return q_size(head);
    }

    q_reverse(head);

    struct list_head *cur = head->next;

    while (cur != head) {
        struct list_head *next_node = cur->next;
        while (next_node != head) {
            element_t *cur_element = list_entry(cur, element_t, list);
            element_t *next_element = list_entry(next_node, element_t, list);

            struct list_head *next_next = next_node->next;
            if (strcmp(cur_element->value, next_element->value) < 0) {
                list_del(&next_element->list);
                free(next_element->value);
                free(next_element);
            }
            next_node = next_next;
        }
        cur = cur->next;
    }

    q_reverse(head);
    return q_size(head);
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    if (!head || list_empty(head) || head->next->next == head) {
        return q_size(head);
    }

    q_reverse(head);

    struct list_head *cur = head->next;

    while (cur != head) {
        struct list_head *next_node = cur->next;
        while (next_node != head) {
            element_t *cur_element = list_entry(cur, element_t, list);
            element_t *next_element = list_entry(next_node, element_t, list);

            struct list_head *next_next = next_node->next;
            if (strcmp(cur_element->value, next_element->value) > 0) {
                list_del(&next_element->list);
                free(next_element->value);
                free(next_element);
            }
            next_node = next_next;
        }
        cur = cur->next;
    }

    q_reverse(head);
    return q_size(head);
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return 0;

    queue_contex_t *first_ctx = list_entry(head->next, queue_contex_t, chain);
    struct list_head *result_queue = first_ctx->q;

    if (list_is_singular(head))
        return first_ctx->size;

    struct list_head *chain_node = head->next->next;
    while (chain_node != head) {
        queue_contex_t *current_ctx =
            list_entry(chain_node, queue_contex_t, chain);
        struct list_head *current_queue = current_ctx->q;
        if (!list_empty(current_queue)) {
            list_splice_tail(current_queue, result_queue);

            first_ctx->size += current_ctx->size;

            INIT_LIST_HEAD(current_queue);
            current_ctx->size = 0;
        }

        chain_node = chain_node->next;
    }

    q_sort(result_queue, descend);

    return first_ctx->size;
    // https://leetcode.com/problems/merge-k-sorted-lists/
}
