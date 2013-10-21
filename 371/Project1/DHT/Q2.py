import sys
#the class for the nodes
class Node:
    """docstring for Node"""
    #constructor
    def __init__(self,id):
        self.id=id;
        self.isJoined=False;
        self.keys=[];
        self.successor=[];
        return;
    #update the successor according to current distribution
    def updateSuccessor(self,dht):
        #find the ith successor while 2^i < DHT' space
        i=0
        self.successor=[];
        while 2**i<len(dht.nodeSpace):
            #print str(i)+"th iteraton";
            #assign the successor if found:
            if(dht.nodeSpace[(self.id+2**i)%len(dht.nodeSpace)]!=0):
                self.successor.append((self.id+2**i)%len(dht.nodeSpace))
            #if not find, try the next slot
            else:
                shift=1
                while shift<=len(dht.nodeSpace):
                    hop=self.id+2**i+shift;
                    pos=hop%len(dht.nodeSpace);
                    #print str(hop)+" "+str(pos)
                    #if find a successor, add it to list
                    if(dht.nodeSpace[pos]!=0):
                        self.successor.append(pos)
                        break
                    #check next slot
                    else:
                        shift=shift+1
            i=i+1
        return
    def addNewKey(self,newKey):
        #print "add"
        self.keys.append(newKey);
        return;

#the class for the whole system
class DHT:
    """docstring for ClassName"""
    def __init__(self, spaceSize):
        #super(ClassName, self).__init__()
        self.nodeSpace = [0 for i in range(spaceSize)];
        self.keySpace=[];
    def joinNewNodes(self, newNodes):
        for i in newNodes:
            node=Node(int(i));
            self.nodeSpace[node.id]=node;
            node.isJoined=True;
        #every node in the DHT updates its successor list;
        for j in self.nodeSpace:
            if j !=0:
                j.updateSuccessor(self);
        return;
    def joinNewKeys(self,newKeys):
        for i in newKeys:
                #start with the positon which equals to the key_id
                #print "saving key"+str(i)
            for k in range(len(self.nodeSpace)):
                pos=(i+k)%len(self.nodeSpace);
                #if pos is a node, then save the key
                #print pos;
                if(self.nodeSpace[pos]!=0):
                    self.nodeSpace[pos].addNewKey(i);
                    break;
        self.keySpace.extend(newKeys);
        return;
    #input:     nodesList contains all the nodes generating the queries in a list
    #           keysList conatain all the keys queried
        #return:A list contains lists of search path of all keys from each node
    def queryKeysFromNodes(self,nodesList,keysList):
        path=[];
        for node in nodesList:
            allPathFromNode=[];
            if(self.nodeSpace[node]!=0):
                for key in keysList:
                    allPathFromNode.append(self.queryKeyFromNode(self.nodeSpace[node],key));
            else:
                allPathFromNode=["Invaild Node."];
            path.append(allPathFromNode);
        return path;
    #input:node that generate the query
    #           key that be fetched
    #output: a list containing the search path
    def queryKeyFromNode(self,node,key):
        if(key in self.keySpace):
            path=[];
            pos=node.id
            while True:
                #record current position;
                path.append(pos);
                currentNode=self.nodeSpace[pos];
                #if key is found, mission completed
                if(key in currentNode.keys):
                    return path;
                #else check the successor list to find out where to go:
                else:
                    #if key equals one element in successor list, then that's it
                    if key in currentNode.successor:
                        pos=key;
                    #else find the larggest successor which is smaller than key
                    else:
                        lst=currentNode.successor[:];
                        for suc in lst:
                            #if suc < pos, it means keys is stored at the nodes with smallest id(after walking through the hole ring)
                            if suc<=pos:
                                lst[lst.index(suc)]=suc+len(self.nodeSpace);
                        #check the list of successor
                        #if the last successor is still smaller than key, then return it;
                        if lst[-1]<key:
                            pos=lst[-1];
                        else:
                            for suc in lst:
                                #if suc>key,
                                if suc>key:
                                    #first suc is bigger than key, no other choice
                                    if lst.index(suc)==0:
                                        pos=currentNode.successor[0];
                                    #the previous successor is the next hop
                                    else:
                                        pos=currentNode.successor[lst.index(suc)-1];
                                    break;

                #print "next pos="+str(pos);
        else:
            return ["Key is not in the DHT system now."];

#the function to generate report
def extractResult(nodeNum,keyNum,resultList):
    print resultList
    f=open("Q2.out",'w');
    for i in range(nodeNum):
        for j in range(keyNum):
            f.write(" ".join(str(i) for i in resultList[i][j])+"\n")
    f.close();


#Driver
#print sys.argv
filename=sys.argv[1]
f=open(filename,'r');
hashSpaceSize=int(f.readline());
numOfNodes=int(f.readline());
numOfKeys=int(f.readline());
joinedNodes=list(int(i) for i in f.readline().strip().split(","));
joinedKeys=list(int(i) for i in  f.readline().strip().split(","));
f.close();
print hashSpaceSize,numOfKeys,numOfNodes,joinedKeys,joinedNodes;



# #Grab all the input info.
# hashSpaceSize=int(raw_input())
# numOfNodes=int(raw_input())
# numOfKeys=int(raw_input())
# joinedNodes=list(int(i) for i in raw_input().split(','));
# joinedKeys=list(int(i) for i in raw_input().split(','));

#Driver function
#Construct the node space
layout=DHT(hashSpaceSize);
#Add new nodes in turn
layout.joinNewNodes(joinedNodes);
#Add new keys in turn
layout.joinNewKeys(joinedKeys);

#print the keys that are currently in the DHT
print layout.keySpace;

#print the figure table of each nodes;
for i in layout.nodeSpace:
    if(i!=0):
        print "node"+str(i.id)+"->"+str(i.successor)+"\t keys="+str(i.keys);
    else:
        print "nil"

#Fetch the keys, result is a list contains all the path
s=layout.queryKeysFromNodes([1],[0,1,2,3,4,5,6,7]);

#Extract the useful info and re-format then write in to the .out file
extractResult(1,8,s);
