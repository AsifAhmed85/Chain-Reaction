
class Pair<Item1 extends Comparable<Item1>, Item2 extends Comparable<Item2>> implements Comparable<Pair<Item1, Item2>> {
    Item1 first;
    Item2 second;

    public Item1 getFirst() {
        return first;
    }

    public Item2 getSecond() {
        return second;
    }

    public Pair(Item1 first, Item2 second) {
        this.first = first;
        this.second = second;
    }

    @Override
    public int compareTo(Pair<Item1, Item2> o) {
        return this.getFirst().compareTo(o.getFirst());
    }
}
