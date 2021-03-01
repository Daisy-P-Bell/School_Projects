import java.util.Iterator;

/**
 * Walker takes an Iterable of Coords and simulates an individual
 * walking along the path over the given Terrain
 */
public class Walker {
    Coord currentPosition;
    float leftOver;
    Iterator<Coord> walkerPath;
    float temp;
    Terrain terr;
    float cost;
    // terrain: the Terrain the Walker traverses
    // path: the sequence of Coords the Walker follows
    public Walker(Terrain terrain, Iterable<Coord> path) {
        walkerPath = path.iterator();
        terr = terrain;
        currentPosition = walkerPath.next();

    }

    // returns the Walker's current location
    public Coord getLocation() {
        return currentPosition;
    }

    // returns true if Walker has reached the end Coord (last in path)
    public boolean doneWalking() {
        if(currentPosition == null){
            return true;
        }
        return false;
    }

    // advances the Walker along path
    // byTime: how long the Walker should traverse (may be any non-negative value)
    public void advance(float byTime) {
        temp = byTime;
        temp = temp + leftOver;
        //Coord tempPath = walkerPath.next();
        Coord temp2;
        while(walkerPath != null) {
            temp2 = walkerPath.next();
            cost = terr.computeTravelCost(temp2, walkerPath.next());
            walkerPath.remove();
            if (temp >= cost) {
                //cost = terr.computeTravelCost(walkerPath, walkerPath.next());
                temp = temp - cost;
                currentPosition = walkerPath.next();
                continue;
            }
            leftOver = temp;
            break;
        }
    }

}