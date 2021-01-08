import java.util.Random;
import java.util.Scanner;
import java.util.concurrent.Semaphore;

public class NewClass extends Thread
{
    static int MAX;
    static Semaphore[] widelec;
    int numb;
    int typ;
    Random randN;
    public NewClass(int nr, int typ)
    {
        numb = nr;
        this.typ = typ;
        if(typ == 3)
            randN = new Random(numb);
    }
    public void run()
    {
        while(true)
        {
            System.out.println("Rozmyślam ¦ " + numb);
            try
            {
                Thread.sleep((long)(7000 * Math.random()));
            }
            catch (InterruptedException e){}
            if(typ == 1)
            {
                widelec[numb].acquireUninterruptibly();
                widelec[(numb + 1)%MAX].acquireUninterruptibly();
            }
            else if(typ == 2)
            {
                if(numb == 0)
                {
                    widelec[(numb + 1)%MAX].acquireUninterruptibly();
                    widelec[numb].acquireUninterruptibly();
                }
                else
                {
                    widelec[numb].acquireUninterruptibly();
                    widelec[(numb + 1)%MAX].acquireUninterruptibly();
                }
            }
            else if(typ == 3)
            {
                int strona = randN.nextInt(2);
                boolean podnioslDwaWidelce = false;
                do {
                    if(strona == 0)
                    {
                        widelec[numb].acquireUninterruptibly();
                        if(!(widelec[(numb + 1)%MAX].tryAcquire()))
                            widelec[numb].release();
                        else
                            podnioslDwaWidelce = true;
                    }
                    else
                    {
                        widelec[(numb + 1)%MAX].acquireUninterruptibly();
                        if(!(widelec[numb].tryAcquire()))
                            widelec[(numb + 1)%MAX].release();
                        else
                            podnioslDwaWidelce = true;
                    }
                } while (!podnioslDwaWidelce);
            }
            System.out.println(("Zasiada do jedzenia " + numb));
            try
            {
                Thread.sleep((long)(5000 * Math.random()));
            }
            catch (InterruptedException e){}
            System.out.println ("Kończy posiłek " + numb) ;
            widelec[numb].release();
            widelec[(numb+1)%MAX].release();
        }
    }

    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        System.out.println("1. Problem filozofów");
        System.out.println("2. Problem ucztujących filozofów z niesymetrycznym sięganiem po widelce");
        System.out.println("3. Problem filozofów z rozwiązaniem rzutem monetą");
        System.out.println("Podaj numer: ");
        int typ = scan.nextInt();
        while(!(typ == 1 || typ == 2 || typ == 3))
        {
            System.out.println("Błąd zakresu!");
            System.out.println("Podaj poprawny numer: ");
            typ = scan.nextInt();
        }

        System.out.println("Ilość Filozofów min 2 max 100: ");
        MAX = scan.nextInt();

        while(MAX > 100 || MAX < 2)
        {
            System.out.println("Błąd zakresu!");
            System.out.println("Ilość Filozofów min 2 max 100: ");
            MAX = scan.nextInt();
        }
        widelec = new Semaphore [MAX];

        for(int i = 0; i < MAX; i++){widelec[i] = new Semaphore(1);}
        for(int i = 0; i < MAX; i++){new NewClass(i, typ).start();}
    }
}
