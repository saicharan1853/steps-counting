#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structure for indi
typedef struct Individual {
    int ID;
    char name[50];
    int age;
    int goal;
    int steps[7];
    struct Individual *left;
    struct Individual *right;
    int height;
} Individual;

// Define structure for group member
typedef struct GroupMember {
    Individual *indi;
    struct GroupMember *next;
} GroupMember;

// Define structure for group
typedef struct Group {
    int groupID;
    char groupName[50];
    GroupMember *members;
    int ggoal;
    struct Group *left;
    struct Group *right;
    int height;
    int sum;//for storing totalsteps
} Group;

Individual *proot = NULL;
    Group *groot = NULL;

// Function to create a new indi root
Individual* create_individual(int ID, char *name, int age, int goal, int *steps) {
    Individual* indi = (Individual*)malloc(sizeof(Individual));
    indi->ID = ID;
    strcpy(indi->name, name);
    indi->age = age;
    indi->goal = goal;
    for (int i = 0; i < 7; i++) { 
        indi->steps[i] = steps[i];
    }
    indi->left = NULL;
    indi->right = NULL;
    indi->height = 0;
    return indi;
}
// Function to create a new group root
Group* create_group(int groupID, char *groupName, int weeklyGoal) {
    Group* group = (Group*)malloc(sizeof(Group));
    group->groupID = groupID;
    strcpy(group->groupName, groupName);
    group->ggoal = weeklyGoal;
    group->members = NULL;
    group->left = NULL;
    group->right = NULL;
    group->height = 0;
    group->sum=0;
    return group;
}
// Function to get height of individual root
int getheight(Individual *root) {
    int h;
    if (root == NULL){
        h=0;
    }
    else{
        h = root->height;
    }
    return h;
}
// Function to get height of group root
int getheightgroup(Group *root) {
     int h;
    if (root == NULL){
        h=0;
    }
    else{
        h = root->height;
    }
    return h;
}


// Get Balance factor of individual root 
int getBalance(Individual *N) {
    int bf;
    if (N == NULL)
    {    bf =0;
    }
    else{
        bf=getheight(N->left) - getheight(N->right);
    }
    return bf;
}
// Get Balance factor of group root 
int getBalancegroup(Group *N) {
    int bf;
    if (N == NULL)
    {    bf =0;
    }
    else{
        bf=getheightgroup(N->left) - getheightgroup(N->right);
    }
    return bf;
}

// Function to get maximum of two integers
int max(int a, int b) {
    int max;
    if(a>b){
        max=a;
    }
    else{
        max=b;
    }
    return max;
}
// Function to right rotate subtree rooted with y
Individual *rightRotate(Individual *y) {
    Individual *x = y->left;
    Individual *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getheight(y->left), getheight(y->right)) + 1;
    x->height = max(getheight(x->left), getheight(x->right)) + 1;

    // Return new root
    return x;
}

// Function to left rotate subtree rooted with x
Individual *leftRotate(Individual *x) {
    Individual *y = x->right;
    Individual *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getheight(x->left), getheight(x->right)) + 1;
    y->height = max(getheight(y->left), getheight(y->right)) + 1;

    // Return new root
    return y;
}
// Function to right rotate subtree rooted with y
Group *rightRotategroup(Group *y) {
    Group *x = y->left;
    Group *T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(getheightgroup(y->left), getheightgroup(y->right)) + 1;
    x->height = max(getheightgroup(x->left), getheightgroup(x->right)) + 1;

    // Return new root
    return x;
}

// Function to left rotate subtree rooted with x
Group *leftRotategroup(Group *x) {
    Group *y = x->right;
    Group *T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(getheightgroup(x->left), getheightgroup(x->right)) + 1;
    y->height = max(getheightgroup(y->left), getheightgroup(y->right)) + 1;

    // Return new root
    return y;
}
// Function to insert a new individual node
Individual* add_person(Individual **proot, Individual *pnode) {
    // Perform the normal BST insertion
    if (*proot == NULL){
       *proot = pnode; // Return the new node if the tree is empty
    }
    if (pnode->ID < (*proot)->ID)
        (*proot)->left = add_person(&((*proot)->left), pnode);
    else if (pnode->ID > (*proot)->ID)
        (*proot)->right = add_person(&((*proot)->right), pnode);

    // Update height of this ancestor node
    (*proot)->height = 1 + max(getheight((*proot)->left), getheight((*proot)->right));

    int balance = getBalance(*proot);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && pnode->ID < (*proot)->left->ID)
        *proot= rightRotate(*proot);

    // Right Right Case
    if (balance < -1 && pnode->ID > (*proot)->right->ID)
       *proot = leftRotate(*proot);

    // Left Right Case
    if (balance > 1 && pnode->ID > (*proot)->left->ID) {
        (*proot)->left = leftRotate((*proot)->left);
       *proot= rightRotate(*proot);
    }

    // Right Left Case
    if (balance < -1 && pnode->ID < (*proot)->right->ID) {
        (*proot)->right = rightRotate((*proot)->right);
        *proot = leftRotate(*proot);
    }

    // Return the unchanged root pointer
    return *proot;
}
// Function to add an individual to a group in the AVL tree

void addMemberToGroup(Group *group, Individual *person) {
    Group *currGroup = group;
    GroupMember *currMember = currGroup->members;
    Individual *current = person;
    int found=0;
    if (currGroup == NULL&&found==0) {
        printf("Group does not exist.\n");
    } else {
        // Check if the group is full
        int count = 0;
        GroupMember *tempMember = currMember;
        while (tempMember != NULL) {
            count++;
            tempMember = tempMember->next;
        }

        if (count >= 5) {
            printf("Group %s is full.\n", currGroup->groupName);
        } else {
            // Check if the person is already in the group
            while (currMember != NULL&&found==0) {
                if (currMember->indi->ID == current->ID) {
                    printf("Person with ID %d already belongs to group %s.\n", current->ID, currGroup->groupName);
                    found=1; // Exit function if person is already in the group
                }
                currMember = currMember->next;
            }
            if(!found){
            // If person is not already in the group, add them
            GroupMember *newMember = (GroupMember *)malloc(sizeof(GroupMember));
            if (newMember != NULL) {
                
            newMember->indi = current;
            newMember->next = currGroup->members;
            currGroup->members = newMember;
            }
            }
        }
    }
}

// Function to insert a new group root
Group* insert_group(Group **groot, Group* gnode) {
    if (*groot == NULL) {
        *groot = gnode;
    }

    if (gnode->groupID < (*groot)->groupID)
        (*groot)->left = insert_group(&((*groot)->left), gnode);
    else if (gnode->groupID > (*groot)->groupID)
        (*groot)->right = insert_group(&((*groot)->right), gnode);

    // Update height of this ancestor root
    (*groot)->height = 1 + max(getheightgroup((*groot)->left), getheightgroup((*groot)->right));

   
    int balance = getBalancegroup(*groot);

    // If this root becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (balance > 1 && gnode->groupID < (*groot)->left->groupID)
        *groot = rightRotategroup(*groot);

    // Right Right Case
    if (balance < -1 && gnode->groupID > (*groot)->right->groupID)
        *groot = leftRotategroup(*groot);

    // Left Right Case
    if (balance > 1 && gnode->groupID > (*groot)->left->groupID) {
        (*groot)->left = leftRotategroup((*groot)->left);
        *groot = rightRotategroup(*groot);
    }

    // Right Left Case
    if (balance < -1 && gnode->groupID < (*groot)->right->groupID) {
        (*groot)->right = rightRotategroup((*groot)->right);
        *groot = leftRotategroup(*groot);
    }

    return *groot; // Return the updated root
}

// Function to search for an individual in the AVL tree
Individual* search_individual(Individual* proot, int ID) {
    //root is NULL or key is present at root
    if (proot == NULL || proot->ID == ID)
        return proot;

    // Key is greater than root's ID
    if (proot->ID < ID){
        search_individual(proot->right, ID);
    }
    else{
    search_individual(proot->left, ID);
    }
}

// Function to search for a group in the AVL tree
Group* search_group(Group* groot, int groupID) {
    //root is NULL or key is present at root
    if (groot == NULL || groot->groupID == groupID)
        return groot;

    // Key is greater than root's groupID
    if (groot->groupID < groupID){
         search_group(groot->right, groupID);
       
     }else{
    search_group(groot->left, groupID);
     }
}

// Function to calculate total steps of an individual
int getTotalSteps(Individual *indi) {
    int totalSteps = 0;
    for (int i = 0; i < 7; i++) {
        totalSteps += indi->steps[i];
    }
    return totalSteps;
}

Individual *top[3] = {NULL};
int topSteps[3] = {0};

// Recursive function to find top 3 individuals
void findTop3(Individual *proot, Individual *top[], int topSteps[], int *count) {
    if (proot != NULL){

    if (proot->goal <= getTotalSteps(proot)) {
        // Check if there's space in top 3 or if the current individual has more steps
        if (*count < 3 || (top[2] && getTotalSteps(proot) > topSteps[2])) {
            // If top 3 is not full, add the current individual
            if (*count < 3) {
                top[*count] = proot;
                topSteps[*count] = getTotalSteps(proot);
                (*count)++;
            } else { // If top 3 is full, replace the individual with the least steps
                top[2] = proot;
                topSteps[2] = getTotalSteps(proot);
            }

            // Bubble up the newly added individual to maintain sorted order
            int index = *count - 1;
            while (index > 0 && topSteps[index] > topSteps[index - 1]) {
                // Swap the individual and its steps with the previous one
                Individual *tempIndividual = top[index];
                top[index] = top[index - 1];
                top[index - 1] = tempIndividual;

                int tempSteps = topSteps[index];
                topSteps[index] = topSteps[index - 1];
                topSteps[index - 1] = tempSteps;

                index--;
            }
        }
    }
    findTop3(proot->right, top, topSteps, count);
    findTop3(proot->left, top, topSteps, count);
}
}

// Function to display the top 3 individuals who have completed their daily step goals
void Get_top_3(Individual *proot) {
    int count = 0;

    findTop3(proot, top, topSteps, &count);

    printf("Top 3 Individuals:\n");
    for (int i = 0; i < count; ++i) {
        printf("ID: %d Name: %s Total Steps: %d\n", top[i]->ID, top[i]->name, topSteps[i]);
    }
}

 // Function to calculate total weekly steps of a group
void calculateGroupTotalSteps(Group *groot) {
    if (groot != NULL) {
        calculateGroupTotalSteps(groot->left);
        calculateGroupTotalSteps(groot->right);

        int totalSteps = 0;
        GroupMember *currentMember = groot->members;
        while (currentMember != NULL) {
            totalSteps += getTotalSteps(currentMember->indi);
            currentMember = currentMember->next;
        }
        groot->sum = totalSteps;
    }
}
// Function to display whether a given group has completed its weekly group goal
void Check_group_achievement(Group *groot, int groupID) {
   
    Group *group = search_group(groot, groupID);
    if (group != NULL) {
          if (group->sum >= group->ggoal)
        printf("Group '%s' has achieved its weekly group goal of %d with %d steps.\n", group->groupName,group->ggoal,group->sum);
    else
        printf("Group '%s' has not achieved its weekly group goal.\n", group->groupName);
    }
    else{
        printf("Group with ID %d not found.\n", groupID);
    }
}
// Function to count the number of nodes in the AVL tree
int countNodes(Group *root) {
    int count;
    if (root == NULL){
        count=0;
    }
    else{
        count = 1 + countNodes(root->left) + countNodes(root->right);
    }
    return count;
}

// Function to merge two sorted arrays of Group pointers
void merge(Group **arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays
    Group **L = (Group **)malloc(n1 * sizeof(Group *));
    Group **R = (Group **)malloc(n2 * sizeof(Group *));
    if (L == NULL || R == NULL) {
        printf("Memory allocation failed\n");
    }else{

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i]->sum >= R[j]->sum)
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    // Copy the remaining elements of L[], if any
    while (i < n1)
        arr[k++] = L[i++];

    // Copy the remaining elements of R[], if any
    while (j < n2)
        arr[k++] = R[j++];
    }
    free(L);
    free(R);
}

// Function to implement merge sort on an array of Group pointers
void mergeSort(Group **arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2; // Calculate the middle index

        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted halves
        merge(arr, l, m, r);
    }
}

void populateArray(Group *root, Group **arr, int *index) {
        if (root != NULL){
        populateArray(root->left, arr, index);
        arr[(*index)++] = root;
        populateArray(root->right, arr, index);
        }
    }

// Function to generate and display the leaderboard
void Generate_leader_board(Group **groot) {
    if (*groot != NULL) {
       
    // Count the number of nodes in the AVL tree
    int count = countNodes(*groot);

    // Allocate memory for the array based on the number of nodes in the AVL tree
    Group **arr = (Group **)malloc(sizeof(Group *) * count);
    if (arr == NULL) {
        printf("Memory allocation failed\n");
    }

    // Populate the array with pointers to groups using an inorder traversal
    int index = 0;
    
    populateArray(*groot, arr, &index);

    // Sort the array based on total steps using merge sort
    mergeSort(arr, 0, count - 1);

    // Display leaderboard
    printf("Leader Board:\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s with total steps: %d\n", i + 1, arr[i]->groupName, arr[i]->sum);
    }

    // Free dynamically allocated memory
    free(arr);
    }else{
    printf("No groups found.\n");
    }
       
}
int calculateGroupRank(Group **groot, int groupID) {
    int rank = -1; // Initialize rank as -1 (indicating group not found)
    
    if (*groot == NULL) {
        printf("No groups found.\n");
        return rank;
    }

    // Count the number of nodes in the AVL tree
    int count = countNodes(*groot);

    // Allocate memory for the array based on the number of nodes in the AVL tree
    Group **arr = (Group **)malloc(sizeof(Group *) * count);
    if (arr == NULL) {
        printf("Memory allocation failed\n");
    }

    // Populate the array with pointers to groups using an inorder traversal
    int index = 0;
    
    populateArray(*groot, arr, &index);

    // Sort the array based on total steps using merge sort
    mergeSort(arr, 0, count - 1);
   
    // Search for the group with the given groupID in the array
    for (int i = 0; i < count; i++) {
        if (arr[i]->groupID == groupID) {
            rank = i + 1; // Adjust rank to start from 1
        }
    }

    // Free dynamically allocated memory
    free(arr);

    return rank;
}

// Function to display rewards earned by an individual
void Check_individual_rewards(Individual *proot, int ID) {
    int count=0;
    findTop3(proot, top, topSteps, &count);
    // Check if root is not NULL before accessing its attributes
    Individual* curr = search_individual(proot,ID);
    
    if (curr != NULL) {
        
        // Calculate total steps of the individual
        int totalSteps = getTotalSteps(curr);
        
        // Check if the individual has met their goal
        if (curr->goal <= totalSteps) {
         int rank;   // Display rewards based on rank
            if (curr->ID==top[0]->ID)
                printf("Individual with ID %d earned 100 points.\n", ID);
            else if (curr->ID==top[1]->ID)
                printf("Individual with ID %d earned 75 points.\n", ID);
            else if (curr->ID==top[2]->ID)
                printf("Individual with ID %d earned 50 points.\n", ID);
            else
                printf("Individual with ID %d reaced their goal but did not earn any points.\n", ID);
        } else {
            printf("Individual with ID %d did not meet their goal.\n", ID);
        }
    } else {
        printf("Error: Root is NULL.\n");
    }
}

Individual* temp1;
void removeFromGroup(Group **groot, int ID) {
    Group *currGroup = *groot;
    if (currGroup != NULL) {
        GroupMember *prev = NULL;
        GroupMember *currIndividual = currGroup->members;

        while (currIndividual != NULL) {
            if (currIndividual->indi->ID == ID) {
                // Remove the individual from the group
                if (prev != NULL) {
                    prev->next = currIndividual->next;
                } else {
                    currGroup->members = currIndividual->next; // Update head if removing the first member
                }
                free(currIndividual->indi); // Free the memory for the individual
                free(currIndividual); // Free the memory for the GroupMember
                return;
            }
            prev = currIndividual;
            currIndividual = currIndividual->next;
        }
        removeFromGroup(&(currGroup->left), ID); // Remove from left subtree
        removeFromGroup(&(currGroup->right), ID); // Remove from right subtree
    }
    if (temp1 != NULL) {
            temp1=NULL;
            free(temp1);
    }
}

Individual* delete_individual(Individual** proot, Group **groot, int ID) {
    if (*proot != NULL){

    if (ID < (*proot)->ID)
        (*proot)->left = delete_individual(&((*proot)->left), groot, ID);
    else if (ID > (*proot)->ID)
        (*proot)->right = delete_individual(&((*proot)->right), groot, ID);
    else if(ID==(*proot)->ID) {
        // Node with one or no child
        Individual *temp;
        if ((*proot)->left == NULL) {
            temp = (*proot)->right;
           temp1=*proot;
            return temp;
        } else if ((*proot)->right == NULL) {
            temp = (*proot)->left;
            temp1=*proot;
            return temp;
        }

        // Node with two children
        temp = (*proot)->right;
        while (temp && temp->left != NULL)
            temp = temp->left;

        (*proot)->ID = temp->ID;
        strcpy((*proot)->name, temp->name);
        (*proot)->age = temp->age;
        (*proot)->goal = temp->goal;
         for (int i = 0; i < 7; i++) { 
         (*proot)->steps[i] = temp->steps[i];
        }
        (*proot)->right = delete_individual(&((*proot)->right), groot, temp->ID);
      }
      removeFromGroup(groot, ID);
    printf("Individual with ID %d deleted\n",ID);
    }else{
         printf("Individual with ID %d not found.\n", ID);
    }
    return *proot;
}

// Function to delete a group but retain its individuals
Group* delete_group(Group** groot, int groupID) {
    // Base case: If the tree is empty
    if (*groot == NULL)
        return *groot;
    // If the key to be deleted is smaller than the groot's key, then it lies in the left subtree
    if (groupID < (*groot)->groupID)
        (*groot)->left = delete_group(&((*groot)->left), groupID);

    // If the key to be deleted is greater than the groot's key, then it lies in the right subtree
    else if (groupID > (*groot)->groupID)
        (*groot)->right = delete_group(&((*groot)->right), groupID);

    // If key is same as groot's key, then This is the node to be deleted
    else {
       // Node with one or no child
        Group *temp;
        if ((*groot)->left == NULL) {
            temp = (*groot)->right;
            *groot=NULL;
            free(*groot);
            return temp;
        } else if ((*groot)->right == NULL) {
            temp = (*groot)->left;
            *groot=NULL;
            free(*groot);
            return temp;
        }

        // Node with two children
        temp = (*groot)->right;
        while (temp && temp->left != NULL)
            temp = temp->left;

        (*groot)->groupID = temp->groupID;
        strcpy((*groot)->groupName, temp->groupName);
        (*groot)->ggoal = temp->ggoal;
        (*groot)->right = delete_group(&((*groot)->right), temp->groupID);
    }
    printf("Group with %d is deleted\n",groupID);
    return *groot;
}
 
// Function to merge two groups and create a new group
Group* merge_groups(Group** groot, int groupID1, int groupID2) {
    
    
    // Search for the groups to merge
    Group *group1 = search_group(*groot, groupID1);
    Group *group2 = search_group(*groot, groupID2);

    // If either of the groups is not found, return
    if (group1 == NULL || group2 == NULL) {
        printf("One or both of the groups to merge were not found.\n");
    }else{
        GroupMember* curr1=group1->members;
        GroupMember* curr2=group2->members;
        int a=0,b=0;
        while(curr1!=NULL){
        a++;
        curr1=curr1->next;
        }
        while(curr2!=NULL){
        a++;
        curr2=curr2->next;
        }
     a+=b;
        if(a > 5){
            printf("The merged group will be too large.\n");
        }
        else{
         // Get input for the new group's ID, name, and goals
        int newid,newGoals;
         char newname[20];
        printf("Enter the new id for the group:");
        scanf("%d", &newid);
        printf("Enter the new name for the group:");
        scanf("%s", newname);
         printf("Enter the new goal for the group:");
        scanf("%d", &newGoals);
   // Create a new group to merge into
    Group *group = create_group(newid,newname,newGoals); 
    // Merge the members of group2 into mergedGroup
    GroupMember *currentMember = group2->members;
    while (currentMember != NULL) {
        addMemberToGroup(group, currentMember->indi);
        currentMember = currentMember->next;
    }

    // Merge the members of group1 into mergedGroup
    currentMember = group1->members;
    while (currentMember != NULL) {
        addMemberToGroup(group, currentMember->indi);
        currentMember = currentMember->next;
    }
    // Delete group1 and group2
    *groot = delete_group(groot, groupID1);
    *groot = delete_group(groot, groupID2);
    // Add the merged group to the main group list
    *groot = insert_group(groot, group); 
    
        }
    }
    return *groot;
}
// Function to display group information including group goals and ranks
void Display_Group_range_info(Group *groot,int groupID1,int groupID2 ) {
 
 
   Group* root1=search_group(groot,groupID1);
   Group* root2=search_group(groot,groupID2);
   if(root1!=NULL&&root2!=NULL){
    int x=root1->groupID;
    int y=root2->groupID;
    for(x ;x <= y; x++){
           Group* root3=search_group(groot,x);
      int rank1 = calculateGroupRank(&groot,x);
        printf("Group Name: %s\n", root3->groupName);
        printf("Group ID: %d\n", root3->groupID);
        printf("Weekly Group Goal: %d\n", root3->ggoal);
        printf("Group Rank: %d\n",rank1);
        printf("Group Members:\n");
        GroupMember *currentMember = root3->members;
        while (currentMember != NULL) {
            if(currentMember->indi!=NULL){
            printf("ID: %d, Name: %s\n", currentMember->indi->ID, currentMember->indi->name);
            
            }
            currentMember = currentMember->next;
        }
        printf("\n");
    }
    }
    else{
    printf("Group with ID %d not found\n",groupID1);

   }
   }

    // Function to calculate average steps per day for an indi
    int calculateAverageSteps() {
        int totalSteps = 0;
        for (int i = 0; i < 3; ++i)
            totalSteps += topSteps[i];
             totalSteps= (totalSteps /21)+1;
             return totalSteps;
    }

// Function to suggest daily goal update for an indi
void Suggest_goal_update(Individual *proot, int ID) {


    // Search for the individual 
    Individual *indi = search_individual(proot, ID);

    if (indi != NULL) {

    // Calculate average steps per day
    int averageSteps = calculateAverageSteps(indi);
        printf("It is suggested daily step goal %d for indi with ID %d.\n",averageSteps, ID);
    }else{
    printf("Individual with ID %d not found.\n", ID);
        
    }
}
void read_data_from_file() {
    FILE *fp = fopen("a.txt", "r");
    if (fp != NULL) {
    int n;
    fscanf(fp, "%d", &n); // Read the number of individuals

    // Read individual data
    for (int i = 0; i < n; i++) {
        int id, age, goal, steps[7];
        char name[50]; 
        fscanf(fp, "%d %s %d %d", &id, name, &age, &goal);
        for (int j = 0; j < 7; j++) {
            fscanf(fp, "%d", &steps[j]);
        }
        Individual *indi = create_individual(id, name, age, goal, steps);
        proot = add_person(&proot, indi); // Insert individual into AVL tree
    }
 int m;
fscanf(fp, "%d", &m); // Read the number of groups (m)
for (int i = 0; i < m; i++) {
    int id, goal;
    char name[50];
    fscanf(fp, "%d %s %d", &id, name, &goal);

    // Create a new group
    Group *group = create_group(id, name, goal);

    for(int i=0;i<5;i++){
    int memberID;
    fscanf(fp, "%d", &memberID);
    if(memberID != 0) {
        // Add member to the group
        Individual *member = search_individual(proot, memberID);
        if (member != NULL) {
            addMemberToGroup(group,member);
        }
    }
    // Insert the group into the group tree (groot)
    groot = insert_group(&groot, group);
        }
    }

    fclose(fp);
    } 
    else{
    printf("Error opening file!\n");
        
    }
}

void inorder(Individual* root){
    if(root!=NULL){
        inorder(root->left);
        printf("ID:%d Name:%s\n",root->ID,root->name);
        inorder(root->right);
    }
}

void inordergroup(Group* root){
    if(root!=NULL){
        inordergroup(root->left);
        printf("GroupID:%d GroupName:%s\n",root->groupID,root->groupName);
        inordergroup(root->right);
    }
}


int main() {
    
    read_data_from_file(proot,groot);
    calculateGroupTotalSteps(groot);
    inorder(proot);
    inordergroup(groot);
    // Test your functions here

    int x,x1,x2,x3,x4,x5,x6,x7;
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
                Get_top_3(proot); 
                printf("------------------------------------------------------------\n\n");
                break;
            case 2:
             printf("enter the id: ");
             scanf("%d",&x);
                Check_group_achievement(groot,x);
                printf("------------------------------------------------------------\n\n");
                break;
            case 3:
               Generate_leader_board(&groot);
                printf("------------------------------------------------------------\n\n");
                break;
            case 4:
             printf("enter the id: ");
             scanf("%d",&x1);
                Check_individual_rewards(proot, x1);
                printf("------------------------------------------------------------\n\n");
                break;
            case 5:
             printf("enter the id: ");
             scanf("%d",&x2);
                delete_group(&groot, x2);
                printf("------------------------------------------------------------\n\n");
              break;
            case 6:
                printf("enter the id: ");
                scanf("%d",&x3);
                delete_individual(&proot,&groot, x3);
                printf("------------------------------------------------------------\n\n");
                break;
            case 7:
             printf("enter the id1 and id2: ");
             scanf("%d %d",&x4,&y);
                merge_groups(&groot, x4, y);
                printf("------------------------------------------------------------\n\n");
                break;
            case 8:
            printf("enter the id1 and id2: ");
             scanf("%d %d",&x5,&x6);
                Display_Group_range_info(groot,x5,x6);
                printf("------------------------------------------------------------\n\n");
                break;
            case 9:
            Get_top_3(proot);
            printf("enter the id: ");
             scanf("%d",&x7);
                Suggest_goal_update(proot, x7);
                printf("------------------------------------------------------------\n\n");
                break;
                   
            case 10:
                printf("Exiting program. Goodbye!\n");
                printf("------------------------------------------------------------\n\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
                printf("------------------------------------------------------------\n\n");
        }
    }

    return 0;
}
