import java.io.IOException;
import java.io.PrintWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.Path;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.List;
import java.util.Scanner;

/**
 * 
 * @author 20204012-7 Cassiano Flores and 20204015-0 Mateus Freitas
 */

public class CadastroPontuacao {

    private class Player {
        private String player;
        private int pontos;
        
        public Player(String player, int pontos) {
            this.player = player;
            this.pontos = pontos;
        }

        public String getPlayer() {
            return player;
        }

        public int getPontos() {
            return pontos;
        }

        @Override
        public String toString() {
            return player + "  ----------------------  " + pontos+" pontos";
        }
      
    }

    private List<Player> pontuacoes;
    private Player p;

    public CadastroPontuacao() {
         pontuacoes = new ArrayList<>();
    }

    public void cadastra(String player, int pontos) {
        p = new Player(player, pontos);
        pontuacoes.add(p);
    }

    public void persiste() {
        //carrega pontuações anteriores
        this.carrega();

        //ordena para as pontuações mais altas altes de persistir o arquivo
        pontuacoes.sort(new Comparator<Player>(){
            @Override
            public int compare(CadastroPontuacao.Player p1, CadastroPontuacao.Player p2) {
                return p2.getPontos() - p1.getPontos();
            }
            });

        String fName = "pontuacoes.dat";
        String currDir = Paths.get("").toAbsolutePath().toString();
        String nameComplete = currDir + "\\" + fName;
        Path path = Paths.get(nameComplete);
        try (PrintWriter writer = new PrintWriter(Files.newBufferedWriter(path, StandardCharsets.UTF_8))) {
            
            //arquivo guardara soh as 10 melhores pontuações
            int pos=0;
            for (Player  p : pontuacoes) {
                if(pos<10){
                String linha = p.player + ";"+ p.pontos;
                writer.println(linha);
                pos++;
            }
        }
        } catch (IOException x) {
            System.err.format("Erro de E/S: %s%n", x);
        }
    }


    public void carrega() {
        String fName = "pontuacoes.dat";
        String currDir = Paths.get("").toAbsolutePath().toString();
        String nameComplete = currDir + "\\" + fName;
        Path path = Paths.get(nameComplete);
        try (Scanner sc = new Scanner(Files.newBufferedReader(path, StandardCharsets.UTF_8))) {
           
            while (sc.hasNext()) {
                 
                String linha = sc.nextLine();
                String dados[] = linha.split(";");
                int pos = 0;
                String player = dados[pos];
                pos++;
                int pontos = Integer.parseInt(dados[pos]);
                pos++;
                this.cadastra(player, pontos);
                }
                
            
        } catch (IOException x) {
            System.err.format("Erro de E/S: %s%n", x);
        }  
    }

    @Override
    public String toString() {
        //chama o carrega antes para ler as 10 melhores pontuações
        carrega();
        String jogadores ="";
        for (Player  p : pontuacoes) {
            jogadores += p.toString()+"\n";
        }

        return jogadores;
    }

    
}
