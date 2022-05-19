import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.*;

@SuppressWarnings("Duplicates")

public class Strategy {
    private char myColor;
    private char enemyColor;
    private final int infinity = 999999;
    private final int negativeInfinity = -999999;


    public Strategy(char myColor, char enemyColor) {
        this.myColor = myColor;
        this.enemyColor = enemyColor;
    }
    public Strategy(){}
    public static void main(String[] args) throws FileNotFoundException {
        String input = "shared_file.txt";
        Scanner scanner = new Scanner(System.in);


        //String playerColor = args[0];
        String playerColor = "R";
        char myColor = playerColor.charAt(0);
        char oppColor = myColor == 'R' ? 'G' : 'R';
        Strategy strategy = new Strategy(myColor, oppColor);
        /*ArrayList<Pair<Integer, Integer>> list = strategy.getNeighbours(5,5);

        for(Pair<Integer, Integer> li: list){
            System.out.println(li.getFirst()+ " "+li.getSecond());
        }*/
        while(true) {
            //File inputFile = new File(input);
            String color = scanner.nextLine();
            if (color.equals(playerColor)) {
                String[][] map = new String[8][8];
                for (int i = 0; i < 8; i++) {
                    for (int j = 0; j < 8; j++) {
                        map[i][j] = scanner.next();
                    }
                }
                Pair<Pair<Integer, Integer>, Integer> best = strategy.alphaBeta(map, 4,4,myColor, strategy.negativeInfinity, strategy.infinity, true);
                PrintWriter writer = null;
                try {
                    writer = new PrintWriter(input, "UTF-8");
                } catch (UnsupportedEncodingException e) {
                    e.printStackTrace();
                }
                writer.println(0);
                writer.println(best.getFirst().getFirst()+" " +best.getFirst().getSecond());
                writer.close();
            /*for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    System.out.print(map[i][j]);
                }
                System.out.println();
            }*/
            }
        }
    }

    public Pair<Pair<Integer, Integer>,Integer> alphaBeta(String[][] map1, int depth, int branch, char myColor, int alpha, int beta, boolean maxPlayer){
        ArrayList<Pair<Pair<Integer,Integer>, Integer>> bestOfBests = heuristic(map1,myColor, branch);
        String[][] map = map1.clone();
        if(depth == 1) return bestOfBests.get(0);
        Pair<Pair<Integer, Integer>, Integer> best = null;
        Pair<Integer, Integer> bestPosition = null;
        int bestValue;
        if(maxPlayer){
            int value = negativeInfinity;
            for(Pair pair : bestOfBests) {
                Pair move = (Pair) pair.first;
                int x = (int) move.getFirst();
                int y = (int) move.getSecond();
                updateMap(map, x, y, myColor);
                Pair<Pair<Integer,Integer>, Integer> ret = alphaBeta(map, depth-1, branch, myColor, alpha, beta, false);
                if(ret.second > value){
                    value = ret.second;
                    bestPosition = ret.getFirst();
                }
                alpha = Math.max(alpha, value);
                if(alpha >= beta) break;
            }
            bestValue = value;
            best = new Pair<>(bestPosition, bestValue);
        }
        else{
            int value = infinity;
            for(Pair pair : bestOfBests) {
                Pair move = (Pair) pair.first;
                int x = (int) move.getFirst();
                int y = (int) move.getSecond();
                updateMap(map, x, y, enemyColor);
                Pair<Pair<Integer,Integer>, Integer> ret = alphaBeta(map, depth-1, branch, myColor, alpha, beta, true);
                if(ret.second < value){
                    value = ret.second;
                    bestPosition = ret.getFirst();
                }
                beta = Math.min(beta, value);
                if(alpha >= beta) break;
            }
            bestValue = value;
            best = new Pair<>(bestPosition, bestValue);
        }
        return best;
    }
    private void updateMap(String[][] map,int x, int y, char color){
        int criticalValue = critical_mass(x, y);
        if(map[x][y].equals("No")){
            String app = String.valueOf(color);
            map[x][y] = app + "1";
        }
        else if(map[x][y].charAt(1)-'0' < criticalValue){
            map[x][y] = color + String.valueOf(map[x][y].charAt(1)-'0' + 1);
        }
        else{
            map[x][y] = "No";
            ArrayList<Pair<Integer, Integer>> list = getNeighbours(x, y);
            for(Pair<Integer, Integer> up : list){
                int p = up.getFirst();
                int q = up.getSecond();
                updateMap(map, p, q, color);
            }
        }
    }

    private ArrayList<Pair<Pair<Integer, Integer>, Integer>> heuristic(String[][] map, char myColor, int branch){
        ArrayList<Pair<Pair<Integer, Integer>, Integer>> bestOfBests = new ArrayList<>();
        PriorityQueue<Pair<Pair<Integer, Integer>, Integer>> sortMe = new PriorityQueue<>(64,Collections.reverseOrder());
        for(int i = 0; i < map.length; i++){
            for(int j = 0; j < map[i].length; j++){
                int score = countPoints(map, myColor);
                if (score == 10000) {
                    bestOfBests.add(new Pair<>(new Pair<>(i,j),score));
                    return bestOfBests;
                }
                sortMe.add(new Pair<>(new Pair<>(i,j),score));
            }
        }
        int counter = 0;
        for(Pair pair : sortMe){
            bestOfBests.add(pair);
            counter++;
            if(counter == branch) return bestOfBests;
        }
        return bestOfBests;
    }
    private int critical_mass(int x, int y){
        if ((x == 0 && y == 0) || (x == 7 && y == 7) || (x == 0 && y == 7) || (x == 7 || y == 0)) return 2;
        else if(x == 0 || x == 7 || y == 0 || y ==7) return 3;
        else return 4;

    }
    private ArrayList<Pair<Integer, Integer>> getNeighbours(int x, int y){
        ArrayList<Pair<Integer, Integer>> list = new ArrayList<>();
        ArrayList<Pair<Integer, Integer>> neighbours = new ArrayList<>();
        neighbours.add(new Pair<>(x,y+1));
        neighbours.add(new Pair<>(x,y-1));
        neighbours.add(new Pair<>(x+1,y));
        neighbours.add(new Pair<>(x-1,y));
        for(Pair pair : neighbours){
            if((int)pair.getFirst() >= 0 && (int)pair.getFirst() < 8 && (int)pair.getSecond() >= 0 && (int)pair.getSecond() < 8)
                list.add(pair);
        }
        return list;
    }
    private ArrayList<Integer> getChainLength(String[][] map1, char myColor){
        String[][] map = map1.clone();
        ArrayList<Integer> chains = new ArrayList<>();
        for(int i = 0; i < map.length; i++){
            for(int j = 0; j < map[i].length; j++){
                String str = map[i][j];
                char color;
                int mass ;
                if(str.equals("No")) {
                    color = 'n';
                    mass = 0;
                }
                else {
                    color = str.charAt(0);
                    mass = str.charAt(1);
                }
                if(mass == critical_mass(i,j)-1 && color == myColor){
                    int length = 0;

                    Stack<Pair<Integer, Integer>> stack = new Stack<>();
                    stack.push(new Pair<>(i, j));
                    while(!stack.isEmpty()){
                        Pair postition = stack.pop();
                        int x = (int) postition.getFirst();
                        int y = (int) postition.getSecond();
                        map[x][y] = "No";
                        length++;
                        ArrayList<Pair<Integer, Integer>> lists = getNeighbours(x,y);
                        for(Pair pair : lists){
                            String str1 = map[x][y];
                            char color1;
                            int mass1 ;
                            if(str1.equals("No")) {
                                color1 = 'n';
                                mass1 = 0;
                            }
                            else {
                                color1 = str1.charAt(0);
                                mass1 = str1.charAt(1);
                            }
                            if(mass1 == critical_mass((int)pair.getFirst(), (int) pair.getSecond())-1 && color1 == myColor)
                                stack.push(new Pair<>((int)pair.first, (int)pair.second));
                        }
                    }
                    chains.add(length);
                }
            }
        }
        return chains;

    }
    private int countPoints(String[][] map, char myColor){
        char opponentColor = myColor == 'G' ? 'R' : 'G';
        int points = 0, myOrbs = 0, enemyOrbs = 0;
        for(int i = 0; i < map.length; i++){
            for(int j = 0; j < map[i].length; j++){
                String str = map[i][j];
                char color;
                int mass ;
                if(str.equals("No")) {
                    color = 'n';
                    mass = 0;
                }
                else {
                    color = str.charAt(0);
                    mass = str.charAt(1);
                }
                if(color == myColor){
                    myOrbs += mass;
                    boolean flag = true;
                    for(Pair pair : getNeighbours(i,j)){
                        int x = (int) pair.getFirst();
                        int y = (int) pair.getSecond();
                        String str1 = map[x][y];
                        char color1;
                        int mass1 ;
                        if(str1.equals("No")) {
                            color1 = 'n';
                            mass1 = 0;
                        }
                        else {
                            color1 = str1.charAt(0);
                            mass1 = str1.charAt(1);
                        }
                        if(color1 == opponentColor && mass1 == critical_mass(x,y)-1){
                            points -= 5-critical_mass(i,j);
                            flag = false;
                        }
                        if(flag){
                            int cMass = critical_mass(i, j);
                            if(cMass == 3) points += 2;
                            else if(cMass == 2) points += 3;
                            if(mass1 == cMass-1) points += 2;
                        }
                    }
                }
                else enemyOrbs += mass;
            }
        }
        points += myOrbs;
        if(enemyOrbs == 0 && myOrbs > 1) return 10000;
        else if(myOrbs == 0 && enemyOrbs > 1) return -10000;
        int sum = 0;
        for(int i : getChainLength(map, myColor)){
            sum += 2 * i;
        }
        points += sum;
        return points;
    }

}
