#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> // For MessageBox on Windows

#define MAX_ENTRIES 100
#define MAX_USER_ENTRIES 10 

typedef struct
{
    int user_id;
    char name[50];
    char date[11];
    float weight;
    int exercise_minutes;
    int calories; 
    float height;
    int diet_calories;  
    int protein_intake; 
    int carb_intake;    
    int fat_intake;     
} HealthEntry;

typedef struct
{
    int user_id;                           // Unique ID for the user
    HealthEntry entries[MAX_USER_ENTRIES]; // Store multiple entries for each user
    int entry_count;                       // Track number of entries for this user
} User;

User users[MAX_ENTRIES];
int user_count = 0;

// Function to find a user by user_id
User *find_user(int user_id)
{
    for (int i = 0; i < user_count; i++)
    {
        if (users[i].user_id == user_id)
        {
            return &users[i];
        }
    }
    return NULL;
}

// Function to generate a unique user ID
int generate_user_id()
{
    return ++user_count;
}

// Function to display notifications using MessageBox (Windows only)
void show_notification(const char *title, const char *message)
{
    MessageBox(NULL, message, title, MB_OK | MB_ICONINFORMATION);
}

// Function to add a new workout entry 
void add_entry()
{
    int user_id;
    int has_unique_id;
    printf("Do you have a unique user ID? (1 for Yes, 0 for No): ");
    scanf("%d", &has_unique_id);

    if (has_unique_id == 0)
    {
        user_id = generate_user_id();
        printf("You don't have a user ID. Your new ID is: %d\n", user_id);
    }
    else
    {
        printf("Enter your unique user ID: ");
        scanf("%d", &user_id);
    }

    User *user = find_user(user_id);
    if (user == NULL)
    {
        user = &users[user_count++];
        user->user_id = user_id;
        user->entry_count = 0;
    }

    if (user->entry_count >= MAX_USER_ENTRIES)
    {
        printf("User has reached the maximum number of entries.\n");
        return;
    }

    HealthEntry new_entry;

    if (has_unique_id == 1)
    {
        // Copy the name from the user's existing data
        strcpy(new_entry.name, user->entries[0].name); // Take the name from the first entry
    }
    else
    {
        printf("Enter your name: ");
        scanf("%s", new_entry.name);
    }

    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", new_entry.date);
    printf("Enter weight (in kg): ");
    scanf("%f", &new_entry.weight);

    // Height entry
    int feet, inches;
    printf("Enter height (feet): ");
    scanf("%d", &feet);
    printf("Enter height (inches): ");
    scanf("%d", &inches);

    // Convert height from foot to meters (1 foot = 0.3048 meters, 1 inch = 0.0254 meters)
    new_entry.height = (feet + inches / 12.0) * 0.3048;

    printf("Enter exercise (in minutes): ");
    scanf("%d", &new_entry.exercise_minutes);
    printf("Enter calories burned: ");
    scanf("%d", &new_entry.calories);

    // Add diet intake
    printf("Enter calories consumed through diet: ");
    scanf("%d", &new_entry.diet_calories);
    printf("Enter protein intake (in grams): ");
    scanf("%d", &new_entry.protein_intake);
    printf("Enter carbohydrate intake (in grams): ");
    scanf("%d", &new_entry.carb_intake);
    printf("Enter fat intake (in grams): ");
    scanf("%d", &new_entry.fat_intake);

    user->entries[user->entry_count++] = new_entry;
    printf("Entry added successfully.\n");

    show_notification("Health Dashboard", "Your health entry has been added successfully.");
}

// Function to view all entries
void view_entries()
{
    int user_id;
    printf("Enter your user ID: ");
    scanf("%d", &user_id);

    User *user = find_user(user_id);
    if (user == NULL)
    {
        printf("No entries found for the given user ID.\n");
        return;
    }

    if (user->entry_count == 0)
    {
        printf("No entries found for the given user ID.\n");
        return;
    }

    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-10s | %-15s | %-10s | %-16s | %-10s | %-8s | %-8s | %-14s | %-14s | %-14s |\n",
           "User ID", "Name", "Date", "Exercise Minutes", "Calories", "Weight", "Height", "Diet Calories", "Protein", "Carbs", "Fats");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < user->entry_count; i++)
    {
        printf("| %-10d | %-15s | %-10s | %-16d | %-10d | %-8.2f | %-8.2f | %-14d | %-14d | %-14d |\n",
               user->entries[i].user_id,
               user->entries[i].name,
               user->entries[i].date,
               user->entries[i].exercise_minutes,
               user->entries[i].calories,
               user->entries[i].weight,
               user->entries[i].height,
               user->entries[i].diet_calories,
               user->entries[i].protein_intake,
               user->entries[i].carb_intake,
               user->entries[i].fat_intake);
    }

    printf("------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

// Function to delete an entry
void delete_entry()
{
    int user_id;
    char date[11];
    printf("Enter your user ID: ");
    scanf("%d", &user_id);

    User *user = find_user(user_id);
    if (user == NULL)
    {
        printf("No entries found for the given user ID.\n");
        return;
    }

    printf("Enter the date of the entry to delete (YYYY-MM-DD): ");
    scanf("%s", date);

    int found = 0;
    for (int i = 0; i < user->entry_count; i++)
    {
        if (strcmp(user->entries[i].date, date) == 0)
        {
            for (int j = i; j < user->entry_count - 1; j++)
            {
                user->entries[j] = user->entries[j + 1];
            }
            user->entry_count--;
            printf("Entry deleted successfully.\n");

            // Show notification after deleting entry
            show_notification("Health Dashboard", "Your health entry has been deleted successfully.");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Entry not found for the given user ID and date.\n");
    }
}

// Function to update an entry
void update_entry()
{
    int user_id;
    printf("Enter your user ID: ");
    scanf("%d", &user_id);

    User *user = find_user(user_id);
    if (user == NULL)
    {
        printf("No entries found for the given user ID.\n");
        return;
    }

    int entry_index;
    printf("Enter the index of the entry to update (0 to %d): ", user->entry_count - 1);
    scanf("%d", &entry_index);

    if (entry_index < 0 || entry_index >= user->entry_count)
    {
        printf("Invalid entry index.\n");
        return;
    }

    printf("Enter new name: ");
    scanf("%s", user->entries[entry_index].name);
    printf("Enter new weight (in kg): ");
    scanf("%f", &user->entries[entry_index].weight);

    int feet, inches;
    printf("Enter height (feet): ");
    scanf("%d", &feet);
    printf("Enter height (inches): ");
    scanf("%d", &inches);

    // Convert height to meters
    user->entries[entry_index].height = (feet + inches / 12.0) * 0.3048;

    printf("Enter new exercise (in minutes): ");
    scanf("%d", &user->entries[entry_index].exercise_minutes);
    printf("Enter new calories burnt: ");
    scanf("%d", &user->entries[entry_index].calories);

    // Add diet intake
    printf("Enter new calories consumed through diet: ");
    scanf("%d", &user->entries[entry_index].diet_calories);
    printf("Enter new protein intake (in grams): ");
    scanf("%d", &user->entries[entry_index].protein_intake);
    printf("Enter new carbohydrate intake (in grams): ");
    scanf("%d", &user->entries[entry_index].carb_intake);
    printf("Enter new fat intake (in grams): ");
    scanf("%d", &user->entries[entry_index].fat_intake);
    printf("Entry updated successfully.\n");

    // Show notification after updating entry
    show_notification("Health Dashboard", "Your health entry has been updated successfully.");
}

// Function to calculate BMI
void calculate_bmi()
{
    int user_id;
    printf("Enter your user ID: ");
    scanf("%d", &user_id);

    User *user = find_user(user_id);
    if (user == NULL)
    {
        printf("No entries found for the given user ID.\n");
        return;
    }

    if (user->entry_count == 0)
    {
        printf("No entries found for the given user ID.\n");
        return;
    }

    printf("User's BMI calculations:\n");
    for (int i = 0; i < user->entry_count; i++)
    {
        float bmi = user->entries[i].weight / (user->entries[i].height * user->entries[i].height);
        printf("Entry %d - Date: %s, Weight: %.2f kg, Height: %.2f meters, BMI: %.2f\n",
               i + 1, user->entries[i].date, user->entries[i].weight, user->entries[i].height, bmi);

        // Provide health advice based on BMI
        if (bmi < 18.5)
        {
            printf("BMI indicates underweight. Increase your calorie intake and consider consulting a doctor for diet recommendations.\n");
        }
        else if (bmi >= 18.5 && bmi < 24.9)
        {
            printf("BMI indicates normal weight. Keep maintaining a balanced diet.\n");

            // Play a congratulatory tune for normal weight BMI (2-second tune)
            Beep(1000, 200); // Frequency 1000 Hz, Duration 200 ms
            Beep(1200, 200); // Frequency 1200 Hz, Duration 200 ms
            Beep(1000, 200); // Frequency 1000 Hz, Duration 200 ms
        }
        else if (bmi >= 25.0 && bmi < 29.9)
        {
            printf("BMI indicates overweight. Consider a balanced diet with more exercise.\n");
        }
        else
        {
            printf("BMI indicates obesity. A strict diet plan and exercise program may be needed.\n");
        }
    }
    show_notification("Health Dashboard", "Your BMI calculations have been done successfully.");
}

// Function to generate Health Reports
void generate_health_report()
{
    int user_id;
    printf("Enter your user ID: ");
    scanf("%d", &user_id);

    User *user = find_user(user_id);
    if (user == NULL)
    {
        printf("No entries found for the given user ID.\n");
        return;
    }

    if (user->entry_count == 0)
    {
        printf("No entries found for the given user ID.\n");
        return;
    }

    char start_date[11], end_date[11];
    printf("Enter the start date for the report (YYYY-MM-DD): ");
    scanf("%s", start_date);
    printf("Enter the end date for the report (YYYY-MM-DD): ");
    scanf("%s", end_date);

    float total_weight = 0;
    int total_exercise_minutes = 0;
    int total_calories = 0;
    int total_diet_calories = 0;
    int total_protein = 0;
    int total_carbs = 0;
    int total_fats = 0;
    int entry_count_in_range = 0;

    for (int i = 0; i < user->entry_count; i++)
    {
        if (strcmp(user->entries[i].date, start_date) >= 0 && strcmp(user->entries[i].date, end_date) <= 0)
        {
            total_weight += user->entries[i].weight;
            total_exercise_minutes += user->entries[i].exercise_minutes;
            total_calories += user->entries[i].calories;
            total_diet_calories += user->entries[i].diet_calories;
            total_protein += user->entries[i].protein_intake;
            total_carbs += user->entries[i].carb_intake;
            total_fats += user->entries[i].fat_intake;
            entry_count_in_range++;

            // Calculate BMI for each entry
            float bmi = user->entries[i].weight / (user->entries[i].height * user->entries[i].height);

            printf("Entry %d - Date: %s, Weight: %.2f kg, Height: %.2f meters, BMI: %.2f\n",
                   i + 1, user->entries[i].date, user->entries[i].weight, user->entries[i].height, bmi);
        }
    }

    if (entry_count_in_range == 0)
    {
        printf("No entries found within the specified date range.\n");
        return;
    }

    // Calculate averages
    float avg_weight = total_weight / entry_count_in_range;
    float avg_exercise_minutes = total_exercise_minutes / (float)entry_count_in_range;
    float avg_calories = total_calories / (float)entry_count_in_range;
    float avg_diet_calories = total_diet_calories / (float)entry_count_in_range;
    float avg_protein = total_protein / (float)entry_count_in_range;
    float avg_carbs = total_carbs / (float)entry_count_in_range;
    float avg_fats = total_fats / (float)entry_count_in_range;

    printf("\nHealth Report for User ID %d\n", user_id);
    printf("-------------------------------------------------------------------------\n");
    printf("Total entries: %d\n", entry_count_in_range);
    printf("Average weight: %.2f kg\n", avg_weight);
    printf("Total exercise minutes: %d minutes\n", total_exercise_minutes);
    printf("Average calories burned: %.2f\n", avg_calories);
    printf("Average diet calories: %.2f\n", avg_diet_calories);
    printf("Average protein intake: %.2f grams\n", avg_protein);
    printf("Average carbohydrate intake: %.2f grams\n", avg_carbs);
    printf("Average fat intake: %.2f grams\n", avg_fats);
    printf("---------------------------------------------------------------------------------\n");

    // Show notification after generating the report
    show_notification("Health Dashboard", "Your Health Report & Analytics have been generated successfully.");
}

int main()
{
    int choice;
    while (1)
    {
        printf("**************************** Health_Dash ****************************\n");
        printf("-----------------------------------------\n");
        printf("| %-4s | %-28s |\n", "Option", "Description");
        printf("-----------------------------------------\n");
        printf("| %-4d | %-30s |\n", 1, "Add Entry");
        printf("| %-4d | %-30s |\n", 2, "Delete Entry");
        printf("| %-4d | %-30s |\n", 3, "Update Entry");
        printf("| %-4d | %-30s |\n", 4, "View Entries");
        printf("| %-4d | %-30s |\n", 5, "Generate Health Report ");
        printf("| %-4d | %-30s |\n", 6, "Calculate BMI");
        printf("| %-4d | %-30s |\n", 7, "Exit");
        printf("-----------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            add_entry();
            break;
        case 2:
            delete_entry();
            break;
        case 3:
            update_entry();
            break;
        case 4:
            view_entries();
            break;
        case 5:
            generate_health_report();
            break;
        case 6:
            calculate_bmi();
            break;
        case 7:
            exit(0);
        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
