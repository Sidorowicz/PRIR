import java.awt.Color;
import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.io.File;

public class NewClass extends Thread {
    final static int N = 4096;
    final static int CUTOFF = 100;
    static int[][] set = new int[N][N];

    public static void main(String[] args) throws Exception {
        // Calculate set
        long startTime = System.currentTimeMillis();

        NewClass[] watki = new NewClass[4];
        for(int i=0; i<4; i++){
            watki[i] = new NewClass(i);
            watki[i].start();
        }

        for (NewClass watek : watki) {
            watek.join();
        }
        BufferedImage img = new BufferedImage(N, N, BufferedImage.TYPE_INT_ARGB);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                int k = set[i][j];
                float level;
                if (k < CUTOFF) {
                    level = (float) k / CUTOFF;
                } else {
                    level = 0;
                }
                Color c = new Color(0, level, 0);
                img.setRGB(i, j, c.getRGB());
            }
        }
        ImageIO.write(img, "PNG", new File("Julian.png"));
    }

    int me;
    int begin;
    int end;

    static final double ratioY = (1.25 - -1.25) / N;
    static final double ratioX = (1.25 - -1.25) / N;
    public NewClass(int me) {
        this.me = me;
        this.begin = (N/4) * me;
        this.end = (N/4) * (me+1);
    }

    public void run() {
        for (int i = begin; i < end; i++) {
            for (int j = 0; j < N; j++) {
                double rzeczywista = i*ratioY + -1.25;
                double urojona = j*ratioX + -1.25;

                Zesp c = new Zesp(-0.123, 0.745);
                Zesp z = new Zesp(rzeczywista, urojona);
                int k = 0;

                while (k < CUTOFF && z.mod() < 2.0) {
                    // z = c + z * z
                    z = c.suma(z.sqr());
                    k++;
                }
                set[i][j] = k;
            }
        }
    }
}

class Zesp{
    private double r;
    private double u;
    public Zesp(double r, double u){
        this.r = r;
        this.u = u;
    }

    Zesp suma(Zesp inna){
        return new Zesp(r + inna.r, u +inna.u);
    }

    Zesp iloczyn(Zesp inna){
        double rzeczywista = r * inna.r - u * inna.u;
        double urojona = r*inna.u + u*inna.r;
        return new Zesp(rzeczywista, urojona);
    }

    double mod(){
        return Math.sqrt(r*r + u*u);
    }

    Zesp sqr(){
        return this.iloczyn(this);
    }
}
