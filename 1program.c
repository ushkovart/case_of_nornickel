#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int ro_of_copper = 8960; 
int ro_of_iron = 7874;
int ro_of_nickel = 8900;
int n = 16;

typedef struct {
        float nickel;
        float volume;
        float mass;
        int   indicator;
}       t_bunker;

typedef struct {
        float copper;
        float iron;
        float nickel;
        float mass;
}       t_part;

int control_of_bunkers(t_bunker *bunker) //функция контроля заполненности бункеров
{
    int i;

    i = 0;
    while (i < n)
    {
        if (bunker[i].indicator == 0)
        {
            return (1);
        }
        i++;
    }
    return (0);
}

int return_indmin(t_bunker *bunker) //служебная функция основного алгоритма
{
    int index;
    int result;
    float min;
    float current;
    int   i;
    int   minind;

    i = 0;
    while (i < n)
    {
        if (bunker[i].indicator == 0)
        {
            minind = i;
            break;
        }
    }
    min = bunker[minind].nickel * bunker[minind].mass;
    index = 1;
    result = 0;
    while (index < n)
    {
        current = bunker[index].nickel * bunker[index].mass;
        if (current < min && bunker[index].indicator == 0)
        {
            min = current;
            result = index;
        }
        index++; 
    }
    return (result); 
}

int return_indmax(t_bunker *bunker) //служебная функция основного алгоритма
{
    int index;
    int result;
    float max;
    float current;
    int i;
    int maxind;

    i = 0;
    while (i < n)
    {
        if (bunker[i].indicator == 0)
        {
            maxind = i;
            break;
        }
    }
    max = bunker[maxind].nickel  / bunker[maxind].mass;
    index = 1;
    result = 0;
    while (index < n)
    {
        current = bunker[index].nickel / bunker[index].mass;
        if (current > max && bunker[index].indicator == 0)
        {
            max = current;
            result = index;
        }
        index++; 
    }
    return (result); 
}

void change_the_array(t_bunker *bunker, int index, t_part part, float volume) //функция обновления данных о бункере
{
    float mass_of_nickel;

    mass_of_nickel = bunker[index].mass * bunker[index].nickel;
    bunker[index].mass += part.mass;
    bunker[index].volume += volume;
    mass_of_nickel += part.nickel * part.mass;
    bunker[index].nickel = mass_of_nickel / bunker[index].mass; 
}

void print_array(t_bunker *bunker) //вывод массива на печать
{
    int i;

    i = 0;
    while (i < n)
    {
        printf("%6.3f ", bunker[i].mass);
        i++;
    }
    printf("\n");
    i = 0;
    /*while (i < n)
    {
        printf("%6.3f ", bunker[i].volume);
        i++;
    }
    printf("\n");*/
    i = 0;
    while (i < n)
    {
        printf("%6.3f ", bunker[i].nickel);
        i++;
    }
    printf("\n");
}

int main() //основная программа 
{
    t_bunker *bunker;
    t_part   part;
    int      mass_of_part;
    float    volume;
    int      counter;
    float    mass_of_nickel;
    float    medium_mass_of_nickel;
    float    summ_of_nickel;
    int      i;
    FILE     *exc;
    int      test;
    
    bunker = malloc(sizeof(t_bunker) * n);
    i = 0;
    while (i < n)
    {
        bunker[i].nickel = 0;
        bunker[i].volume = 0;
        bunker[i].mass = 0;
        bunker[i].indicator = 0;
        i++;
    }
    summ_of_nickel = 0;
    test = 0;
    exc = fopen("result.txt", "w");
    counter = 1;
    srand(time(NULL));
    while (control_of_bunkers(bunker))
    {
        printf("Mass: "); //ввод данных
        scanf("%f", &part.mass); 
        printf("Copper: ");
        scanf("%f", &part.copper);
        printf("Iron: ");
        scanf("%f", &part.iron);
        printf("Nickel: ");
        scanf("%f", &part.nickel);
        volume = part.mass * (part.copper / ro_of_copper + part.iron / ro_of_iron + part.nickel / ro_of_nickel);
        mass_of_nickel = part.nickel * part.mass; //вычисление необходимых параметров в режиме реального времени  
        summ_of_nickel += mass_of_nickel;
        medium_mass_of_nickel = summ_of_nickel / counter;
        if (counter <= n) //алгоритм принятия решений, случай первых 16 партий
        {
            printf("bunker: %d", counter); //вывод данных
            printf("\n");
            change_the_array(bunker, counter - 1, part, volume);
            print_array(bunker);
        }
        else if ((part.nickel * part.mass) >= medium_mass_of_nickel) //случай, когда никеля в партии выше среднего
        {
            i = return_indmin(bunker);
            printf("bunker: %d", i + 1); //вывод данных
            printf("\n");
            change_the_array(bunker, i, part, volume);
            print_array(bunker);
        }
        else //случай, когда никеля в партии ниже среднего
        {
            i = return_indmax(bunker);
            printf("bunker: %d", i + 1); //вывод данных
            printf("\n");
            change_the_array(bunker, i, part, volume);
            print_array(bunker);
        }
        counter++;
    }
    i = 0;
    while (i < n)
    {
        fprintf(exc, "%6.3f ", bunker[i].nickel); //вывод данных в файл
        i++;
    }
    fclose(exc);
}