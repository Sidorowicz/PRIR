import java.util.Random;

class Miejsce_parkingowe {
    static int Parking = 1;
    static int Start_Trasy = 2;
    static int Kurs = 3;
    static int Koniec_Trasy = 4;
    static int Kolizja = 5;

    int ilosc_miejsc;
    int ilosc_zajetych;
    int ilosc_taxówek;

    Miejsce_parkingowe(int ilosc_miejsc, int ilosc_helikopterow)
    {
        this.ilosc_miejsc = ilosc_miejsc;
        this.ilosc_taxówek = ilosc_helikopterow;
        this.ilosc_zajetych = 0;
    }
    synchronized int start(int numer)
    {
        ilosc_zajetych--;
        System.out.println("Klient wzywa taxi: " + numer);
        return Start_Trasy;
    }
    synchronized int laduj()
    {
        try
        {
            Thread.currentThread().sleep(1000);
        }
        catch (Exception e)
        {

        }
        if(ilosc_zajetych < ilosc_miejsc)
        {
            ilosc_zajetych++;
            System.out.println("Klient kończy kurs : " + ilosc_zajetych);
            return Parking;
        }
        else
            return Koniec_Trasy;
    }
    synchronized void zmniejsz()
    {
        ilosc_taxówek--;
        System.out.println("Smiertelny wypadek");
        if(ilosc_taxówek == ilosc_miejsc)
            System.out.println("Ilosc taxówek jest taka sama jak ilosc miejsc");
    }
}

class Taxi extends Thread {
    static int Parking = 1;
    static int Start_kursu = 2;
    static int Kurs = 3;
    static int Koniec_kursu = 4;
    static int Wypadek = 5;
    static int Zatankuj = 1000;
    static int Końcówka_paliwa = 500;

    int numer;
    int paliwo;
    int stan;
    Miejsce_parkingowe l;
    Random rand;
    Kabiniarz p;

    public Taxi(int numer, int paliwo, Miejsce_parkingowe l)
    {
        this.numer = numer;
        this.paliwo = paliwo;
        this.stan = Kurs;
        this.l = l;
        rand = new Random();
        p = new Kabiniarz(numer);
    }

    public void run()
    {
        p.start();
        while(true)
        {
            if(stan == Parking)
            {
                p.aktualnyStan = stan;
                if(rand.nextInt(2) == 1)
                {
                    stan = Start_kursu;
                    paliwo = Zatankuj;
                    System.out.println("Klinet dzwoni po taxi nr: " + numer);
                    stan = l.start(numer);
                }
                else
                {
                    System.out.println("Czekam na telefon");
                }
            }
            else if(stan == Start_kursu)
            {
                p.aktualnyStan = stan;
                System.out.println("Wyjechało taxi nr: " + numer);
                stan = Kurs;
            }
            else if(stan == Kurs)
            {
                p.aktualnyStan = stan;
                paliwo -= rand.nextInt(500);
                System.out.println("Taxi nr: " + numer + " kursuje");
                if(paliwo<=Końcówka_paliwa)
                {
                    stan = Koniec_kursu;
                }
                else try
                {
                    sleep(rand.nextInt(1000));
                }
                catch(Exception e) { }
            }
            else if(stan == Koniec_kursu)
            {
                p.aktualnyStan = stan;
                System.out.println("kończe kurs nr: " + numer + " mam  " + paliwo+" paliwa");
                stan = l.laduj();
                if(stan == Koniec_kursu)
                {
                    paliwo-= rand.nextInt(500);
                    System.out.println("Końcówka " + paliwo);
                    if(paliwo<=0)
                        stan = Wypadek;
                }
            }
            else  if(stan == Wypadek)
            {
                System.out.println("Wypadek taxi nr: " + numer);
                p.aktualnyStan = stan;
                l.zmniejsz();
            }

        }
    }
}

class Kabiniarz extends Thread {
    Random rand = new Random();
    static int parking = 1;
    static int start_kursu = 2;
    static int kurs = 3;
    static int koniec_kursu = 4;
    static int wypadek = 5;
    int numerKabiny;
    int aktualnyStan;
    Kabiniarz(int numerK)
    {
        this.numerKabiny = numerK;
    }
    public void run()
    {
        while (true)
            try
            {
                sleep(rand.nextInt(1000));
                if(aktualnyStan == kurs)
                    System.out.println("Kierowca kabiny nr " + numerKabiny + " wiezie klienta");
                else if(aktualnyStan == start_kursu)
                    System.out.println("Kierowca kabiny nr " + numerKabiny + " opuszcza parking");
                else if(aktualnyStan == koniec_kursu)
                    System.out.println("Kierowca kabiny nr " + numerKabiny + " konczy kurs");
                else if(aktualnyStan == parking)
                    System.out.println("Kierowca kabiny nr " + numerKabiny + " na parkingu");
                else if(aktualnyStan == wypadek)
                    System.out.println("Kierowca kabiny nr " + numerKabiny + " miał wypadek");
            }
            catch (Exception e){}
    }
}

public class NewClass {
    static int ilosc_kursantów = 5;
    static int ilosc_miejsc = 2;
    static Miejsce_parkingowe parking;
    public static void main(String[] args) {
        parking = new Miejsce_parkingowe(ilosc_miejsc, ilosc_kursantów);
        for(int i = 0; i < ilosc_kursantów; i++)
            new Taxi(i, 2000, parking).start();
    }
}




