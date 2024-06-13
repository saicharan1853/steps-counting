#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// structure for persons
typedef struct person{
    int id;
    char name[20];
    int age;
    int goal;
    int steps[7];
    struct person *next;
}person;
// structure for group
typedef struct group{
    int gid;
    char gname[50];
    person *members[5];
    int size;
    int ggoal;
    struct group* next;
    int sum;
}group;

person* phead = NULL;
group* ghead = NULL;

// creating person
person *create_person(int id, char name[], int age, int goal, int steps[]) {
    person *new =malloc(sizeof(struct person));
    new->id = id;
    strcpy(new->name, name);  
    new->age = age;
    new->goal = goal;  
    for (int i = 0; i < 7; i++) { 
        new->steps[i] = steps[i];
    }
    new->next = NULL;   
    return new;  
}
// creating group
group *create_group(int id, char *name, int goal, int *mem) {
    group *new_group = (group *)malloc(sizeof(group));
    new_group->gid = id;
    strcpy(new_group->gname, name);
    new_group->ggoal = goal;
    new_group->next = NULL;

    int k = 0;
    person *pnode = phead;

    while (pnode != NULL) {
        int flag = 0;
        for (int i = 0; i < 5; i++) {
            if (pnode->id == mem[i] && flag == 0) {
                new_group->members[k] = pnode;
                k++;
                flag = 1; 
            }else{
                new_group->members[k]=NULL;
            
            }
        }
        pnode = pnode->next;
    }

    new_group->size = k;

    return new_group;
}
// Function for creating linked list
void add_person(person **phead, person *pnode) {
    if (*phead == NULL || (*phead)->id > pnode->id) {
        pnode->next = *phead;
        *phead = pnode;
    } else {
        person *current = *phead;
        while (current->next != NULL && current->next->id < pnode->id) {
            current = current->next;
        }
        pnode->next = current->next;
        current->next = pnode;
    }
}

void insert_group(struct group **ghead, struct group *gnode) {
    if (*ghead == NULL || (*ghead)->gid > gnode->gid) {
        gnode->next = *ghead;
        *ghead = gnode;
    } else {
        struct group *curr = *ghead;
        while (curr->next != NULL && curr->next->gid < gnode->gid) {
            curr = curr->next;
        }
        gnode->next = curr->next;
        curr->next = gnode;
    }
}

// Add an existing person to a group
void add_person_to_group(person *phead, group *ghead, int pid, int gid) {
    person *pnode = phead;
    int foundPerson = 0;
    while (pnode != NULL) {
        if (pnode->id == pid) {
            foundPerson = 1;
            
        }
        pnode = pnode->next;
    }
    if (!foundPerson) {
        printf("Person with ID %d does not exist.\n", pid);
        return;
    }

    group *curr = ghead;
    int foundGroup = 0;
    while (curr != NULL) {
        int foundMember = 0;
        for (int i = 0; i < curr->size; i++) {
            if (curr->members[i]->id == pid) {
                printf("Person with ID %d already belongs to group %s.\n", pid, curr->gname);
                foundMember = 1;
                
            }
        }
        if (foundMember) {
            foundGroup = 1;
            
        }
        curr = curr->next;
    }
    if (foundGroup) {
        return;
    }

    group *gnode = ghead;
    while (gnode != NULL && gnode->gid != gid) {
        gnode = gnode->next;
    }
    if (gnode == NULL) {
        printf("Group with ID %d does not exist.\n", gid);
        return;
    }
    if (gnode->size == 5) {
        printf("Group %s is full.\n", gnode->gname);
        return;
    }
    gnode->members[gnode->size] = pnode;
    gnode->size++;
    printf("Person %s added to group %s.\n", pnode->name, gnode->gname);
}

// Function to merge two sorted linked lists of persons based on id 
person *pmerge(person *phead1, person *phead2) {
    person *ptr1, *ptr2, *result, *tail;
    
    // If one of the lists is empty, return the other list
    if (phead2 == NULL) {
        result = phead1;
    } else if (phead1 == NULL) {
        result = phead2;
    } else {
        ptr1 = phead1;
        ptr2 = phead2;
        
        // Determine the head of the merged list based on id
        if (phead1->id < phead2->id) {
            result = phead1;
            ptr1 = ptr1->next;
        } else {
            result = phead2;
            ptr2 = ptr2->next;
        }
        
        // Traverse both lists and merge them based on id
        tail = result;
        while ((ptr1 != NULL) && (ptr2 != NULL)) {
            if (ptr1->id < ptr2->id) {
                tail = tail->next = ptr1;
                ptr1 = ptr1->next;
            } else {
                tail->next = ptr2;
                tail = tail->next;
                ptr2 = ptr2->next;
            }
        }
        
        // Append remaining elements if any
        if (ptr1 != NULL) {
            tail->next = ptr1;
        } else {
            tail->next = ptr2;
        }
    }
    return result;
}

// Function to divide the linked list into two halves
person *pdivide(person *phead) {
    person *fast, *slow, *ptr;
    slow = phead;
    fast = phead->next->next;
    // Move fast pointer twice as fast as the slow pointer
    while (fast != NULL) {
        fast = fast->next;
        slow = slow->next;
        if (fast != NULL) {
            fast = fast->next;
        }
    }
    ptr = slow->next;
    slow->next = NULL;
    return ptr;
}

/* Function to perform merge sort on the linked list */
person *psort(person *phead) {
    person *ptr;
    //  if list is empty or has only one element
    if ((phead != NULL) && (phead->next != NULL)) {
        // Divide the list into two halves
        ptr = pdivide(phead);
        // Recursively sort both halves
        phead = psort(phead);
        ptr = psort(ptr);
        // Merge the sorted halves
        phead = pmerge(phead, ptr);
    }
    return phead;
}

person* top[3] = {NULL, NULL, NULL}; // Array to store the top 3 nodes globally
int total[3] = {0, 0, 0};

void get_top_3(person* phead) {
    phead=psort(phead);
    person* curr = phead; 
 
    while (curr->next!= NULL) { 
        int sum = 0; 
        for (int i = 0; i < 7; i++) { 
            sum += curr->steps[i]; // Add the steps to the sum
        }

        if (sum > curr->goal) {// Check if the current sum qualifies for top 3
            if (sum > total[0]) { // If the sum is greater than the highest total
                // Shift the elements in the top array
                top[2] = top[1];
                total[2] = total[1];
                top[1] = top[0];
                total[1] = total[0];
                top[0] = curr; // Insert the curr node at index 0
                total[0] = sum; // Update the sum at index 0
            } else if (sum > total[1]&& sum < total[0]) { // If the sum is greater than the 2nd highest total
                // Shift the elements in the top array
                top[2] = top[1];
                total[2] = total[1];
                top[1] = curr; // Insert the curr node at index 1
                total[1] = sum; // Update the sum at index 1
            } else if (sum > total[2]&& sum < total[1]) { // If the sum is greater than the 3rd highest total
                top[2] = curr; // Insert the curr node at index 2
                total[2] = sum; // Update the sum at index 2
            }
        }
        curr = curr->next; 
    }

    printf("The top 3 persons are:\n");
    for (int i = 0; i < 3; i++) { 
        if (top[i] != NULL) { 
            printf("%d. %s with %d steps\n", i + 1, top[i]->name, total[i]);
        }
    }
}
void check_group_achievement(int id1) {
    group *gnode = ghead; 
    while (gnode != NULL && gnode->gid != id1) { // Traverse the list until the group is found or the end is reached
        gnode = gnode->next;
    }
    if (gnode == NULL) { // If the group is not found
        printf("Group with ID %d does not exist.\n", id1);
        return;
    }
    int total_steps = 0;
    for (int i = 0; i < gnode->size; i++) {
        for (int j = 0; j < 7; j++) {
            total_steps += gnode->members[i]->steps[j];
        }
    }
    if (total_steps >= gnode->ggoal) {
        printf("Group %s has achieved its weekly goal of %d with %d steps.\n", gnode->gname, gnode->ggoal,total_steps);
    } else {
        printf("Group %s has not achieved its weekly goal of %d. Total steps: %d\n", gnode->gname,gnode->ggoal, total_steps);
    }
}
/* Function to merge two sorted linked lists of groups based on sum of steps */
group *Gmerge(group *ghead1, group *ghead2) {
    group *ptr1, *ptr2, *result, *tail;
    
    // If one of the lists is empty, return the other list
    if (ghead2 == NULL) {
        return ghead1;
    } else if (ghead1 == NULL) {
        return ghead2;
    } else {
        ptr1 = ghead1;
        ptr2 = ghead2;
        
        // Determine the head of the merged list based on the sum of steps
        if (ghead1->sum > ghead2->sum) {
            result = ghead1;
            ptr1 = ptr1->next;
        } else {
            result = ghead2;
            ptr2 = ptr2->next;
        }
        
        // Traverse both lists and merge them based on the sum of steps
        tail = result;
        while ((ptr1 != NULL) && (ptr2 != NULL)) {
            if (ptr1->sum > ptr2->sum) {
                tail = tail->next = ptr1;
                ptr1 = ptr1->next;
            } else {
                tail->next = ptr2;
                tail = tail->next;
                ptr2 = ptr2->next;
            }
        }
        
        // Append remaining elements if any
        if (ptr1 != NULL) {
            tail->next = ptr1;
        } else {
            tail->next = ptr2;
        }
    }
    return result;
}

/* Function to divide the linked list into two halves */
group *divide(group *ghead) {
    group *fast, *slow, *ptr;
    slow = ghead;
    fast = ghead->next->next;
    // Move fast pointer twice as fast as the slow pointer
    while (fast != NULL) {
        fast = fast->next;
        slow = slow->next;
        if (fast != NULL) {
            fast = fast->next;
        }
    }
    ptr = slow->next;
    slow->next = NULL;
    return ptr;
}

/* Function to perform merge sort on the linked list */
group *Gsort(group *ghead) {
    group *ptr;
    // Base case: if list is empty or has only one element
    if ((ghead != NULL) && (ghead->next != NULL)) {
        // Divide the list into two halves
        ptr = divide(ghead);
        // Recursively sort both halves
        ghead = Gsort(ghead);
        ptr = Gsort(ptr);
        // Merge the sorted halves
        ghead = Gmerge(ghead, ptr);
    }
    return ghead;
}

/* Function to generate leader board based on sorted list */
void generate_leader_board(struct group **ghead) {
    struct group *curr = *ghead;
    
    // Calculate sum of steps for each group
    while (curr!= NULL) {
        int sum = 0;
        for (int i = 0; i < curr->size; i++) {
            for (int j = 0; j < 7; j++) {
                sum += curr->members[i]->steps[j];
            }
        }
        curr->sum = sum;
        curr = curr->next;
    }
    
    // Sort the list based on sum of steps
    *ghead = Gsort(*ghead);
    
    // Print the leader board
    printf("The leader board is:\n");
    int rank = 1;
    curr = *ghead;
    while (curr->next!= NULL) {
        printf("%d. %s with %d steps\n", rank, curr->gname, curr->sum);
        rank++;
        curr = curr->next;
    }
}

void check_individual_rewards(struct person *phead, int pid) {
    struct person *pnode = phead; 
    while (pnode != NULL && pnode->id != pid) { 
        pnode = pnode->next;
    }
    if (pnode == NULL) { // If the person is not found
        printf("Person with ID %d does not exist.\n", pid);
        return;
    }
        int flag1 = 0; // Flag to indicate if the person has completed the daily goals
    for (int i = 0; i < 7; i++) {
        if (pnode->steps[i] > pnode->goal&&flag1==0) { // If the steps are less than the goal
            flag1 = 1; // Set the flag to 1
                    }
    }
    if (flag1 == 0) { // If the person has not completed the daily goals
        printf("Person %s has not completed the daily goals.\n", pnode->name);
        return;
    }
    
    int rank = 0;  
    int points = 0;  
    int flag2=0;
    for (int i = 0; i < 3; i++) { // Loop through the top 3 arrays
        if (top[i] == pnode&&flag2==0) { // If the person is found
            rank = i + 1; // Assign the rank
            if (rank == 1) { // If the rank is 1
                points = 100; // Assign 100 points
            } else if (rank == 2) { // If the rank is 2
                points = 75; // Assign 75 points
            } else if (rank == 3) { // If the rank is 3
                points = 50; // Assign 50 points
            }
            flag2=1; // Break the loop
        }
    }
    if (rank == 0) { // If the person is not in the top 3
        printf("Person %s completed the daily goals but not in the top 3 persons.\n", pnode->name);
    } else { // If the person is in the top 3
        printf("Person %s is in the rank %d and has earned %d points.\n", pnode->name, rank, points);
    }
}

void delete_person(person** phead, int id) {
    person* temp = *phead, *prev = NULL;
    
    // Find the node with the given ID
    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    
    // If the node with the given ID is found
    if (temp != NULL && temp->id == id) {
        // If the node to be deleted is the first node
        if (prev == NULL) {
            *phead = temp->next; // Update the head pointer
        } else {
            prev->next = temp->next; // Link the previous node to the next node
        }
        
        free(temp); // Free the memory of the deleted node
        printf("Individual with ID %d deleted.\n", id);
    } else {
        printf("Individual with ID %d not found.\n", id);
    }
}


void delete_group(struct group **ghead, int gid) {
    struct group *gnode = *ghead; // Start from the head of the group list
    struct group *prev = NULL; // Pointer to the previous node
    
    // Traverse the list until the group is found or the end is reached
    while (gnode != NULL && gnode->gid != gid) {
        prev = gnode; // Update the previous node
        gnode = gnode->next; // Move to the next node
    }
    
    if (gnode == NULL) { // If the group is not found
        printf("Group with ID %d does not exist.\n", gid);
        return;
    }
    
    if (prev == NULL) { // If the group is the head of the list
        *ghead = gnode->next; // Make the next node the head
    } else { // If the group is not the head of the list
        prev->next = gnode->next; // Link the previous node to the next node
    }
    
    // Decrement the size
    if ((*ghead) != NULL) {
        (*ghead)->size--;
    }
    
    free(gnode); // Free the memory of the group node
    printf("Group with ID %d deleted.\n", gid);
}
// function for merging two groups
void merge_groups(struct group **ghead, struct person *phead, int gid1, int gid2) {
    struct group *gnode1 = *ghead;
    struct group *prev1 = NULL;

    while (gnode1 != NULL && gnode1->gid != gid1) {
        prev1 = gnode1;
        gnode1 = gnode1->next;
    }

    if (gnode1 == NULL) {
        printf("group with ID %d does not exist.\n", gid1);
        return;
    }

    struct group *gnode2 = *ghead;
    struct group *prev2 = NULL;

    while (gnode2 != NULL && gnode2->gid != gid2) {
        prev2 = gnode2;
        gnode2 = gnode2->next;
    }

    if (gnode2 == NULL) {
        printf("group with ID %d does not exist.\n", gid2);
        return;
    }

    if (gnode1->size + gnode2->size > 5) {
        printf("The merged group will be too large.\n");
        return;
    }

    int new_id; // new ID for the merged group
    char new_name[20]; // new name for the merged group
    int new_goal; // goals
    printf("enter the new id for the new group:");
    scanf("%d", &new_id);
    printf("enter the new name for the new group:");
    scanf("%s", new_name);
    printf("enter the new goal for the new group:");
    scanf("%d", &new_goal);

    // Allocate memory for the new members array
    int new_mem[5] = {0, 0, 0, 0, 0};
      int k=0;
    // Copy members from the first group
    for (int i = 0; i < gnode1->size; i++) {
        new_mem[k] = gnode1->members[i]->id; 
        k++;
    }

    // Copy members from the second group
    for (int i = 0; i < gnode2->size; i++) {
        new_mem[k] = gnode2->members[i]->id;
        k++;
    }
    struct group *new_group = create_group(new_id, new_name, new_goal, new_mem);
    new_group->size=k;

    if (prev1 == NULL) {
        *ghead = gnode1->next;
    } else {
        prev1->next = gnode1->next;
    }

    if (prev2 == NULL) {
        *ghead = gnode2->next;
    } else {
        prev2->next = gnode2->next;
    }

    insert_group(ghead, new_group);
    printf("New group %s created by merging group %s and group %s.\n", new_group->gname, gnode1->gname, gnode2->gname);
    printf("The groups with ids %d %d are deleted\n",gnode1->gid,gnode2->gid);
    free(gnode1);
    free(gnode2);
}
// function for displaying group info
void display_group_info(struct group *ghead, int id1) {
    struct group *gnode = ghead; 
    while (gnode != NULL && gnode->gid != id1) { 
        gnode = gnode->next;
    }
    if (gnode == NULL) { // If the group is not found
        printf("Group with ID %d does not exist.\n", id1);
        return;
    }
    printf("Group name: %s\n", gnode->gname); // Print the group name
    printf("Group ID: %d\n", gnode->gid); // Print the group ID
    printf("Group goal: %d\n", gnode->ggoal); // Print the group goal
    printf("Group size: %d\n", gnode->size); // Print the group size
    printf("Group members:\n"); // Print the group members
    for (int i = 0; i < gnode->size; i++) { // Loop through the members array
        printf("%d. %s (ID: %d)\n", i + 1, gnode->members[i]->name, gnode->members[i]->id); // Print the name and ID of each member
    }
    int rank = 0; // Variable to store the rank of the group
    int sum = 0; // Variable to store the sum of steps for the group
    for (int i = 0; i < gnode->size; i++) { // Loop through the members array
        for (int j = 0; j < 7; j++) { // Loop through the steps array
            sum += gnode->members[i]->steps[j]; // Add the steps to the sum
        }
    }
    struct group *curr = ghead; // Start from the head of the group list
    while (curr != NULL) { // Traverse the list
        int total = 0; // Variable to store the total steps for the curr node
        for (int i = 0; i < curr->size; i++) { // Loop through the members array
            for (int j = 0; j < 7; j++) { // Loop through the steps array
                total += curr->members[i]->steps[j]; // Add the steps to the total
            }
        }
        if (total > sum) { // If the total is greater than the sum
            rank++; // Increment the rank
        }
        curr = curr->next; // Move to the next node
    }
    rank++; // Increment the rank
    printf("Group rank: %d\n\n", rank); // Print the group rank
}
// Suggest a daily goal update
void suggest_goal_update(struct person *phead, int pid) {
    struct person *pnode = phead; 
    while (pnode != NULL && pnode->id != pid) { // Traverse the list until the person is found or the end is reached
        pnode = pnode->next;
    }
    if (pnode == NULL) { // If the person is not found
        printf("Person with ID %d does not exist.\n", pid);    
        return;
    }
    int sum = 0; 
    int flag = 0; 
    for(int i = 0; i < 7; i++){ // Loop through the steps array
        sum += pnode->steps[i]; // Add the steps to the sum
        if (pnode->steps[i] > pnode->goal&& flag==0) { // If the steps are less than the goal
            flag = 1; // Set the flag to 0
        }
    }
    if(flag == 0){ // If the person has not completed the daily goals
        printf("Person %s has not completed the daily goals.\n", pnode->name);
        return;
    }
    int max = 0; // Variable to store the average steps of the top 3 persons
    for(int i=0;i<3;i++){
        max += total[i];
    }
     max=max/21;
    int new_goal = max + 1; // Calculate the new goal as one more than the minimum
    printf("The suggested daily goal update for person %s is %d.\n", pnode->name, new_goal); // Print the new goal
}
// function to read the data from input file
void read_data_from_file() {
    FILE *file = fopen("a.txt", "r");

    if (file == NULL) {
       printf("file is not opened");
    }

    int n, m;
    fscanf(file, "%d", &n);
    for (int i = 0; i < n; i++) {
        int id, age, goal, steps[7];
        char name[20];
        fscanf(file, "%d %s %d %d", &id, name, &age, &goal);
        for (int j = 0; j < 7; j++) {
            fscanf(file, "%d", &steps[j]);
        }
        person *pnode = create_person(id, name, age, goal, steps);
        if (phead == NULL) {
            phead = pnode;
        } else {
            pnode->next = phead;
            phead = pnode;
        }
    }

    fscanf(file, "%d", &m);
    for (int i = 0; i < m; i++) {
        int id, goal, members[5];
        char name[20];
        fscanf(file, "%d %s %d", &id, name, &goal);
        for (int j = 0; j < 5; j++) {
            fscanf(file, "%d", &members[j]);
        }
        group *gnode = create_group(id, name, goal, members);
    if (ghead == NULL) {
        ghead = gnode;
    } else {
        gnode->next = ghead;
        ghead = gnode;
    }
    
    }

    fclose(file);
}

int main() {
   phead=malloc(sizeof(person));
   ghead=malloc(sizeof(group)); 

    if (phead == NULL || ghead == NULL) {
        printf( "Memory allocation error\n");
        return 0;
    }
 
    ghead->size = 0;
    read_data_from_file(phead, ghead); // Read data from file
    phead=psort(phead);


    int x,x1,x2,x3,x4,x5,x6;
    int y;

  int choice;
   while(choice!=10) {
        printf("\nMenu:\n");
        printf("1. Get top 3 individuals\n");
        printf("2. Check group achievement\n");
        printf("3. Generate leader board\n");
        printf("4. Check individual Rewards\n");
        printf("5. Delete group \n");
        printf("6. Delete individual\n");
        printf("7. Merge Groups\n");
        printf("8. Display group info\n");
        printf("9.suggest Goal update\n");
        printf("10.Exit\n");
        printf("------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        printf("------------------------------------------------------------\n");

        switch (choice) {
            case 1:
                get_top_3(phead); 
                printf("------------------------------------------------------------\n\n");
                break;
            case 2:
             printf("enter the id: ");
             scanf("%d",&x);
                check_group_achievement(x);
                printf("------------------------------------------------------------\n\n");
                break;
            case 3:
                generate_leader_board(&ghead);
                printf("------------------------------------------------------------\n\n");
                break;
            case 4:
             printf("enter the id: ");
             scanf("%d",&x1);
                check_individual_rewards(phead, x1);
                printf("------------------------------------------------------------\n\n");
                break;
            case 5:
             printf("enter the id: ");
             scanf("%d",&x2);
                delete_group(&ghead, x2);
                printf("------------------------------------------------------------\n\n");
              break;
            case 6:
                printf("enter the id: ");
                scanf("%d",&x3);
                delete_person(&phead, x3);
                printf("------------------------------------------------------------\n\n");
                break;
            case 7:
             printf("enter the id1 and id2: ");
             scanf("%d %d",&x4,&y);
                merge_groups(&ghead, phead, x4, y);
                printf("------------------------------------------------------------\n\n");
                break;
            case 8:
            printf("enter the id: ");
             scanf("%d",&x5);
                display_group_info(ghead, x5);
                printf("------------------------------------------------------------\n\n");
                break;
            case 9:
            get_top_3(phead);
            printf("enter the id: ");
             scanf("%d",&x6);
                suggest_goal_update(phead, x6);
                printf("------------------------------------------------------------\n\n");
                break;
                   
            case 10:
                printf("Exiting program. Goodbye!\n");
                printf("------------------------------------------------------------\n\n");
                free(phead); //free allocated memory before exiting
                free(ghead);
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                printf("------------------------------------------------------------\n\n");
        }
    }
    return 0;
}
