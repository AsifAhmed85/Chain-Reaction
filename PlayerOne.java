import java.util.*;
@SuppressWarnings("Duplicates")

class playerOne {
    private char player_color;
    private char enemyColor;
    private final int infinity = 999999;
    private final int negativeInfinity = -999999;
    private int playerCount;
    private int opponentCount;
    public Pair<Integer, Integer> bestMove;


    public playerOne(char player_color, char enemyColor) {
        this.player_color = player_color;
        this.enemyColor = enemyColor;
        this.playerCount = 0;
        this.opponentCount = 0;
        this.bestMove = null;
    }

    public playerOne() {
    }

    public static void main(String[] args) {

        String playerColor = args[0];
        char myColor = playerColor.charAt(0);
        char oppColor = (myColor == 'R') ? 'G' : 'R';
        playerOne strategy = new playerOne(myColor, oppColor);
        Scanner scanner = new Scanner(System.in);
        while (!scanner.hasNextLine()) {
        }
        String command = scanner.nextLine();
        while (command.equals("start")) {
            String[][] map = new String[8][8];
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    while (!scanner.hasNext()) {
                    }
                    map[i][j] = scanner.next();
                }
            }
            strategy.alphaBeta(map, 8, 5, myColor, strategy.negativeInfinity, strategy.infinity, true);
            Pair<Integer, Integer> best = strategy.bestMove;
            int x = best.getFirst();
            int y = best.getSecond();
            System.out.println(x + " " + y);
        }
    }

    public int alphaBeta(String[][] map1, int depth, int branch, char myColor, int alpha, int beta, boolean maxPlayer) {
        if (alreadyWon(map1, myColor) && player_color == myColor) return 20000;
        else if(alreadyWon(map1, myColor)) return -20000;
        String[][] map = new String[8][8];
        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                map[i][j] = map1[i][j];
            }
        }

        ArrayList<Pair<Integer, Pair<Integer, Integer>>> bestOfBests = heuristic(map, myColor, branch);
        char opponentColor = (myColor == 'G') ? 'R' : 'G';
        if(bestOfBests.size() == 0 && maxPlayer) return 10000;
        if(bestOfBests.size() == 0 && !maxPlayer) return -10000;
        if (depth == 1) return bestOfBests.get(0).getFirst();
        Pair<Integer, Integer> bestPosition = null;
        int bestValue;
        if (maxPlayer) {
            int value = negativeInfinity;
            for (Pair pair : bestOfBests) {
                Pair move = (Pair) pair.getSecond();
                int x = (int) move.getFirst();
                int y = (int) move.getSecond();
                countOrbs(map, myColor);
                updateMap(map, x, y, myColor);
                int ret = alphaBeta(map, depth - 1, branch, opponentColor, alpha, beta, false);
                if (ret > alpha) {
                    value = ret;
                    alpha = ret;
                    bestPosition = move;
                }
                if (alpha >= beta) break;
            }
            bestValue = value;
        } else {
            if (alreadyWon(map1, myColor)) return  -20000;
            int value = infinity;
            for (Pair pair : bestOfBests) {
                Pair move = (Pair) pair.getSecond();
                int x = (int) move.getFirst();
                int y = (int) move.getSecond();
                countOrbs(map, myColor);
                updateMap(map, x, y, myColor);
                int ret = alphaBeta(map, depth - 1, branch, opponentColor, alpha, beta, true);
                if (ret < beta) {
                    value = ret;
                    beta = ret;
                    bestPosition = move;
                }
                if (alpha >= beta) break;
            }
            bestValue = value;
        }
        this.bestMove = bestPosition;
        return bestValue;
    }


    private ArrayList<Pair<Integer, Pair<Integer, Integer>>> heuristic(String[][] map, char color, int branch) {
        ArrayList<Pair<Integer, Pair<Integer, Integer>>> bestOfBests = new ArrayList<>();
        ArrayList<Pair<Integer, Pair<Integer, Integer>>> bestOfBests2 = new ArrayList<>();

        int score;
        if (player_color == color) {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    if(map[i][j].charAt(0) == color || map[i][j].equals("No")) {
                        score = countPointsUpdated(map, i, j, color);
                        bestOfBests.add(new Pair<>(score, new Pair<>(i, j)));
                    }
                }
            }
        }
        else {
            for (int i = 0; i < map.length; i++) {
                for (int j = 0; j < 8; j++) {
                    if(map[i][j].charAt(0) == color || map[i][j].equals("No")) {
                        score = countPointsUpdated(map, i, j, color) * (-1);
                        bestOfBests.add(new Pair<>(score, new Pair<>(i, j)));
                    }
                }
            }

        }
        Collections.sort(bestOfBests, Collections.reverseOrder());
        for(int i = 0; i < branch && i < bestOfBests.size(); i++){
            bestOfBests2.add(bestOfBests.get(i));
        }

        return  bestOfBests2;
    }

    private int critical_mass(int x, int y) {
        if ((x == 0 && y == 0) || (x == 7 && y == 7) || (x == 0 && y == 7) || (x == 7 && y == 0)) return 1;
        else if (x == 0 || x == 7 || y == 0 || y == 7) return 2;
        else return 3;

    }

    private ArrayList<Pair<Integer, Integer>> getNeighbours(int x, int y) {
        ArrayList<Pair<Integer, Integer>> list = new ArrayList<>();
        ArrayList<Pair<Integer, Integer>> neighbours = new ArrayList<>();
        neighbours.add(new Pair<>(x, y + 1));
        neighbours.add(new Pair<>(x, y - 1));
        neighbours.add(new Pair<>(x + 1, y));
        neighbours.add(new Pair<>(x - 1, y));
        for (Pair pair : neighbours) {
            if ((int) pair.getFirst() >= 0 && (int) pair.getFirst() < 8 && (int) pair.getSecond() >= 0 && (int) pair.getSecond() < 8)
                list.add(pair);
        }
        return list;
    }

    private int countPointsUpdated(String[][] map, int x, int y, char myColor) {


        if (canWin(map, x, y, myColor)) return 20000;
        char opponentColor = (myColor == 'G' ? 'R' : 'G');
        int points;

        String str = map[x][y];
        char color;
        int mass;
        int criticalMass = critical_mass(x, y);
        int factor1 = 0, factor2 = 0, factor3 = 0, factor4 = 0, factor5 = 0, factor6 = 0, factor7 = 0;
        if (str.equals("No")) {
            color = 'n';
            mass = 0;
        } else {
            color = str.charAt(0);
            mass = str.charAt(1)-'0';

        }
        if (mass == criticalMass) {

            factor1 = totalOppExplodes(map, x, y, color);
            factor2 = totalMyExplodes(map, x, y, color);
            ArrayList<Pair<Integer, Integer>> neighbours = getNeighbours(x,y);
            for(Pair<Integer, Integer> pair : neighbours){
                int p = pair.getFirst();
                int q = pair.getSecond();
                int massNeighbour;
                char colorNeighbour;
                if (map[p][q].equals("No")) {
                    colorNeighbour = 'n';
                    massNeighbour = 0;
                } else {
                    colorNeighbour = map[p][q].charAt(0);
                    massNeighbour = map[p][q].charAt(1)-'0';
                }
                if(massNeighbour == critical_mass(p,q) && colorNeighbour != color) factor6++;
                if(colorNeighbour == color) {
                    for (Pair<Integer, Integer> pairn : getNeighbours(p, q)) {
                        int r = pairn.getFirst();
                        int s = pairn.getSecond();
                        int massNeighbourn;
                        char colorNeighbourn;
                        if (map[r][s].equals("No")) {
                            colorNeighbourn = 'n';
                            massNeighbourn = 0;
                        } else {
                            colorNeighbourn = map[r][s].charAt(0);
                            massNeighbourn = map[r][s].charAt(1) - '0';
                        }
                        if (massNeighbourn == critical_mass(r, s) && colorNeighbourn != color) factor7 += 2;
                    }
                }
            }
        } else {
            ArrayList<Pair<Integer, Integer>> neighbours = getNeighbours(x, y);
            boolean noCritical = true;
            for (Pair<Integer, Integer> pair : neighbours) {
                int p = pair.getFirst();
                int q = pair.getSecond();
                int massNeighbour;
                char colorNeighbour;
                if (map[p][q].equals("No")) {
                    colorNeighbour = 'n';
                    massNeighbour = 0;
                } else {
                    colorNeighbour = map[p][q].charAt(0);
                    massNeighbour = map[p][q].charAt(1)-'0';
                }
                if (massNeighbour == critical_mass(p, q) && colorNeighbour == opponentColor) {
                    factor3++;
                    noCritical = false;
                }
            }
            if (noCritical) {
                for (Pair<Integer, Integer> pair : neighbours) {
                    int p = pair.getFirst();
                    int q = pair.getSecond();
                    int massNeighbour;
                    char colorNeighbour;
                    if (map[p][q].equals("No")) {
                        colorNeighbour = 'n';
                        massNeighbour = 0;
                    } else {
                        colorNeighbour = map[p][q].charAt(0);
                        massNeighbour = map[p][q].charAt(1) - '0';
                    }
                    if ((criticalMass - mass) < (critical_mass(p, q) - massNeighbour) && colorNeighbour == opponentColor) {
                        factor4++;
                    }
                }
                if (x == 0) factor5++;
                else if (x == 7) factor5++;
                if (y == 0) factor5++;
                else if (y == 7) factor5++;
            }
        }
        points = factor1 * 20 + factor2 * 4 - factor3 * 8 + factor4 * 4 + factor5 * 4 + factor6 * 10 - factor7 * 25;
        return points;

    }

    private int totalOppExplodes(String[][] map, int x, int y, char color) {
        String[][] mapDuplicate = new String[8][8];
        playerCount = 0;
        opponentCount = 0;
        char opponentColor = (color == 'R' ? 'G' : 'R');

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                mapDuplicate[i][j] = map[i][j];
                if (mapDuplicate[i][j].charAt(0) == color) playerCount++;
                else if (mapDuplicate[i][j].charAt(0) == opponentColor) opponentCount++;
            }
        }
        int initialOpCount = opponentCount;
        updateMap(mapDuplicate, x, y, color);
        return initialOpCount - opponentCount;
    }
    private int totalMyExplodes(String[][] map, int x, int y, char color){
        String[][] mapDuplicate = new String[8][8];
        playerCount = 0;
        opponentCount = 0;
        char opponentColor = (color == 'R' ? 'G' : 'R');

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                mapDuplicate[i][j] = map[i][j];
                if (mapDuplicate[i][j].charAt(0) == color) playerCount++;
                else if (mapDuplicate[i][j].charAt(0) == opponentColor) opponentCount++;
            }
        }
        int initialCount = playerCount;
        updateMap(mapDuplicate, x, y, color);
        if(map[0][0].charAt(0) == color && mapDuplicate[0][0].charAt(0) != color) return 0;
        if(map[0][7].charAt(0) == color && mapDuplicate[0][7].charAt(0) != color) return 0;
        if(map[7][0].charAt(0) == color && mapDuplicate[7][0].charAt(0) != color) return 0;
        if(map[7][7].charAt(0) == color && mapDuplicate[7][7].charAt(0) != color) return 0;
        return playerCount - initialCount;
    }

    private void updateMap(String[][] map, int x, int y, char color) {
        if (opponentCount <= 0 || playerCount < -1) return;
        String str = map[x][y];
        char color2;
        int mass;
        if (str.equals("No")) {
            color2 = 'n';
            mass = 0;
        } else {
            color2 = str.charAt(0);
            mass = str.charAt(1)-'0';
        }
        int criticalValue = critical_mass(x, y);
        if (map[x][y].equals("No")) {
            map[x][y] = String.valueOf(color) + "1";
            playerCount++;
        } else if (mass < criticalValue) {
            if (color2 != color) {
                playerCount++;
                opponentCount--;
            }
            map[x][y] = color + String.valueOf(mass+ 1);
        } else {
            map[x][y] = "No";
            if(color2 != color) {
                opponentCount--;
            } else playerCount--;

            ArrayList<Pair<Integer, Integer>> list = getNeighbours(x, y);
            for (Pair<Integer, Integer> up : list) {
                int p = up.getFirst();
                int q = up.getSecond();
                updateMap(map, p, q, color);
            }
        }
    }

    private boolean canWin(String[][] map, int x, int y, char color) {
        String[][] mapDuplicate = new String[8][8];
        playerCount = 0;
        opponentCount = 0;
        char opponentColor = (color == 'R' ? 'G' : 'R');

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                mapDuplicate[i][j] = map[i][j];
                if (mapDuplicate[i][j].charAt(0) == color) playerCount++;
                else if (mapDuplicate[i][j].charAt(0) == opponentColor) opponentCount++;
            }
        }
        updateMap(mapDuplicate, x, y, color);
        if(playerCount == 1 && opponentCount == 0) return false;
        return ( (playerCount < -1 || opponentCount <= 0));
    }

    private boolean alreadyWon(String[][] map, char color) {
        playerCount = 0;
        opponentCount = 0;
        char opponentColor = (color == 'R' ? 'G' : 'R');
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (map[i][j].charAt(0) == color) playerCount++;
                else if (map[i][j].charAt(0) == opponentColor) opponentCount++;
            }
        }
        return (playerCount != 0 && opponentCount == 0);
    }
    private void countOrbs(String[][] map, char color){
        playerCount = 0;
        opponentCount = 0;
        char opponentColor = (color == 'R' ? 'G' : 'R');
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                if (map[i][j].charAt(0) == color) playerCount++;
                else if (map[i][j].charAt(0) == opponentColor) opponentCount++;
            }
        }
    }
}