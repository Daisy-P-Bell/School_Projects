import java.lang.IndexOutOfBoundsException;
import java.lang.IllegalArgumentException;
import java.util.Stack;

/**
 * Pathfinder uses A* search to find a near optimal path
 * between to locations with given terrain.
 */

public class Pathfinder {
    Coord startCoord;
    Coord endCoord;
    LinkedQueue<Coord> q;
    Terrain terrain;
    float pathCost;
    float h;
    MinPQ<PFNode> minQue;
    Stack<Coord> retStack;
    int counter;
    boolean[][] beenHere;
    boolean pathFound;


    /**
     * PFNode will be the key for MinPQ (used in computePath())
     */
    private class PFNode implements Comparable<PFNode> {
        // loc: the location of the PFNode
        // fromNode: how did we get here? (linked list back to start)
        int getIresult;
        int getJresult;
        Coord loc;
        PFNode fromNode;
        float fromCost;

        public PFNode(Coord loc, PFNode fromNode) {
            getIresult = loc.getI();
            getJresult = loc.getJ();
            this.loc = loc;
            this.fromNode = fromNode;
            if(fromNode != null) {
                fromCost = fromNode.getCost(0);
            }

        }


        // compares this with that, used to find minimum cost PFNode
        public int compareTo(PFNode that) {                                        //this is theoretically done
            return Float.compare(this.getCost(h), that.getCost(h));
        }

        // returns the cost to travel from starting point to this
        // via the fromNode chain
        public float getCost(float heuristic) {                                                //this is done I think
            if(this.fromNode == null){
                return 0;
            }
            float cost = this.fromCost;
            cost = cost + heuristic * terrain.computeDistance(loc, endCoord);
            cost = cost + terrain.computeTravelCost(fromNode.loc, loc);
            //PFNode thisTemp = this;

            /*
            while(!thisTemp.loc.equals(startCoord)){
                cost = cost + heuristic + terrain.computeTravelCost(thisTemp.getIresult, thisTemp.getJresult, thisTemp.fromNode.getIresult, thisTemp.fromNode.getJresult);
                thisTemp = thisTemp.fromNode;
            }
             */
            return cost;
        }

        // returns if this PFNode is not marked invalid
        public boolean isValid() {
            return false;
        }

        // marks the PFNode as invalid
        public void invalidate() {
        }

        // returns if the PFNode is marked as used
        public boolean isUsed() {
            return true;
        }

        // marks the PFNode as used
        public void use() { }

        // returns an Iterable of PFNodes that surround this
        public Iterable<PFNode> neighbors() {
            Stack<PFNode> s = new Stack<>();
            s.push(new PFNode(null, null));
            return s;
        }
    }

    public Pathfinder(Terrain terrain) { //this is done
        this.terrain = terrain;
    } //this si done?

    public void setPathStart(Coord loc) { //this is done
        startCoord = loc;
    }     //This is done

    public Coord getPathStart() {        //this is done
        return startCoord;
    }            //This is done

    public void setPathEnd(Coord loc) {  //this is done
        endCoord = loc;
    }         //this is done

    public Coord getPathEnd() {         //this is done
        return endCoord;
    }                //this is done

    public void setHeuristic(float v) {   //this is done               I don't think this is actually done either
        h = v;
    }                  //this is done??                no I don't think this is done after all

    public float getHeuristic() {
        return h;
    }                     //this is done

    public void resetPath() {                                     //I think this is done
        minQue = new MinPQ<>();
        q = new LinkedQueue<>();
        retStack = new Stack<>();
        pathFound = false;
        counter = 0;
    }

    public void computePath() {
        q =  new LinkedQueue<>();
        minQue = new MinPQ<>();
        //Coord cur = startCoord;
        PFNode PFtemp = new PFNode(startCoord, null);
        int N = terrain.getN();
        beenHere = new boolean[N][N];
        minQue.insert(PFtemp);
        PFNode temp;
        pathFound = false;
        while(true){
            temp = minQue.delMin();
            if((temp.getIresult == endCoord.getI()) && (temp.getJresult == endCoord.getJ())){
                pathFound = true;
                break;
            }
            if(beenHere[temp.getIresult][temp.getJresult] == true){
                continue;

            }
            counter++;
            beenHere[temp.getIresult][temp.getJresult] = true;

            if(temp.getIresult < N-1){
                PFNode nextNode = new PFNode((temp.loc).add(1,0),temp);
                minQue.insert(nextNode);
            }
            if(temp.getIresult -1 >= 0){
                PFNode nextNode = new PFNode((temp.loc).add(-1,0),temp);
                minQue.insert(nextNode);
            }
            if(temp.getJresult < N-1){
                PFNode nextNode = new PFNode((temp.loc).add(0,1),temp);
                minQue.insert(nextNode);
            }
            if(temp.getJresult -1 >= 0){
                PFNode nextNode = new PFNode((temp.loc).add(0,-1),temp);
                minQue.insert(nextNode);
            }
        }
        System.out.print("made it out ");
        retStack = new Stack<>();
        while(!temp.loc.equals(startCoord)) {
            retStack.push(temp.loc);
            temp = temp.fromNode;
        }
        retStack.push(startCoord);
        while(retStack.isEmpty() != true){
            q.enqueue(retStack.pop());
        }

        /*
        while(!cur.equals(endCoord)){
            q.enqueue(cur);
            if(cur.getI() < endCoord.getI()){
                cur = cur.add(1,0);
            }
            else if(cur.getI() > endCoord.getI()){
                cur = cur.add(-1, 0);
            }
            else if(cur.getJ() < endCoord.getJ()){
                cur = cur.add(0, 1);
            }
            else{
                cur = cur.add(0, -1);
            }z
        }
         */
        //q.enqueue(startCoord);
        //q.enqueue(endCoord);
        // compute path cost
        pathCost = terrain.computeTravelCost(q);
    }

    public boolean foundPath() {                       //this is done
        if(pathFound == true){
            return true;
        }
        return false;

    }

    public float getPathCost() {
        return pathCost;
    }    //this is done

    public int getSearchSize() {
        return counter;
    }     //this is done

    public Iterable<Coord> getPathSolution() {  //this is done?
        return q;
    }  //this is done??

    public boolean wasSearched(Coord loc) {               //this is done
        if(beenHere[loc.getI()][loc.getJ()] == true){
            return true;
        }
        return false;
    }
}
