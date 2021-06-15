
extern void os_load();

void boot()
{
    /*init bootrom required hw*/
    
    /*init rom security*/

    /*check signatures*/

    /*check adresses*/

    /*check states*/

    os_load();

    while (1)
    {
        /*Never go out of here!*/
    }
}