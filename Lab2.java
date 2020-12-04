import java.util.function.Function;
interface Num {
    public static double oblicz(double startt, double endd, int n, Function<Double, Double> f)
    {
        return -1;
    };
}
class M_Prostokat extends Num {
    public static double oblicz(double startt, double endd, int dividers, Function<Double, Double> f) {
        M_Prostokatow[] watki = new M_Prostokatow[dividers];
        for (int i = 0; i < dividers; i++) {
            watki[i] = new M_Prostokatow(startt, endd, dividers, i, f);
            watki[i].start();
        }

        double wynik = 0;
        for (int i = 0; i < watki.length; i++) {
            try {
                watki[i].join();
                wynik += watki[i].getWynik();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        return wynik;
    }
}

class M_Sipsona extends Num {
    public static double oblicz(double startt, double endd, int dividers, Function<Double, Double> f) {
        ArrayList<M_Simpsona> watki = new ArrayList<M_Simpsona>();

        for (int i = 0; i < 2; i++) {
            M_Simpsona watek = new M_Simpsona(startt, endd, dividers, i, M_Simpsona.Rodzaj.Zwykly, f);
            watek.start();
            watki.add(watek);
        }

        for (int i = 1; i <= dividers - 1; i++) {
            M_Simpsona watek = new M_Simpsona(startt, endd, dividers, i, M_Simpsona.Rodzaj.Xi, f);
            watek.start();
            watki.add(watek);
        }

        for (int i = 1; i <= dividers; i++) {
            M_Simpsona watek = new M_Simpsona(startt, endd, dividers, i, M_Simpsona.Rodzaj.Ti, f);
            watek.start();
            watki.add(watek);
        }

        double wynik = 0;
        try {
            for (M_Simpsona watek : watki) {
                watek.join();
                wynik += watek.getWynik();
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return wynik;
    }
}
class M_Trapez extends Num {
    public static double oblicz(double startt, double endd, int dividers, Function<Double, Double> f) {
        M_Trapezow[] watki = new M_Trapezow[dividers + 1];
        for (int i = 0; i < watki.length; i++) {
            watki[i] = new M_Trapezow(startt, endd, dividers, i, f);
            watki[i].start();
        }

        double wynik = 0;
        for (int i = 0; i < watki.length; i++) {
            try {
                watki[i].join();
                wynik += watki[i].getWynik();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

        }

        return wynik;
    }
}


public class Zad1 {
    public static void main(String[] args)
    {
        double startt = 0;
        double endd = 1000*Math.PI;
        int n = 100;
        Function<Double, Double> f = (Double x) -> {return Math.pow(x, 2) * Math.sin(-5*x) + 2;};
        System.out.println("Wynik za pomocą metody prostokątów: " + M_Prostokat.oblicz(startt, endd, n, f););
        System.out.println("Wynik za pomocą metody trapezow: " + M_Prostokat.oblicz(startt, endd, n, f););        
        System.out.println("Wynik za pomocą metody simpsona: " + M_Sipsona.oblicz(startt, endd, n, f););  
    }
}
