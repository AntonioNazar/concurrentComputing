import java.util.Random;
class Produtor_Consumidor{
    static final int N=10;
    private double[] buffer= new double[N];
    private int cout=0, in=0, out=0;
    //construtor
    public Produtor_Consumidor(){
        for(int i=0;i<N;i++){
            buffer[i]=0;
        }
    }
    //Imprimir
    public synchronized void Imprime(){
        for(int i=0;i<N;i++){
        System.out.println("Valor do elemento na posicao " 
    +i+"= " + buffer[i]);
        }
        System.out.println("\n");
    }
    //Produtor
    public synchronized void Insere(){
        try {
            Random gerador = new Random();
            double elemento = gerador.nextDouble();
            while(cout==N){
                wait();
            }
            buffer[in]=elemento;
            in=(in+1)%N;
            cout++;
            notifyAll();
        } catch(InterruptedException e){ return;}
    }
    //Consumidor
    public synchronized double Retira(){
        double elemento;
        try {
            while(cout==0){
                wait();
            }
            elemento=buffer[out%N];
            buffer[out%N]=0;
            out=(out+1%N);
            cout--;
            notifyAll();
            } catch(InterruptedException e){return -1;}
        return elemento;
    }
}
class T extends Thread{
    Produtor_Consumidor Pc;
    int index;
    //construtor
    public T(int index,Produtor_Consumidor PC){
        this.Pc=PC;
        this.index=index;
    }
    public void run(){
        int valor = index;
        if(valor==0){
            Pc.Retira();
        }
        else {
            
            Pc.Insere();
        }
        Pc.Imprime();
    }
}
class P_C{
    static final int P = 10;
    static final int C = 10;
    public static void main (String[] args) {
        //reserva espaço para um vetor de threads
        Thread[] threads = new Thread[P+C];
  
        //cria uma instancia do recurso compartilhado entre as threads
        Produtor_Consumidor aux= new Produtor_Consumidor();
        //cria as threads da aplicacao
        for (int i=0; i<P; i++) {
           threads[i] = new T(1, aux);
        }
        for (int i = P; i < P+C; i++) {
            threads[i] = new T(0, aux);
        }
    
        //inicia as threads
        for (int i=0; i<P+C; i++) {
           threads[i].start();
        }
        //espera pelo termino de todas as threads
        for (int i=0; i<P+C; i++) {
            try { threads[i].join(); } catch (InterruptedException e) { return;}
        }
    }
}