# ChikaGossip
Gossip protocol implementation in C++ based on Server/Client codebase.

## Goals
This project intends to setup a gossip protocol with rumor mongering. Since this is a de-centralized protocol, all nodes are treated equal and all nodes should be able to send and receive data to and from other nodes. Membership of the network is treated as the data and thus exchanged among the network. And instead of exchanging the entire membership, each node also holds a temporary set of membership that has recently been pushed into the local membership. These *hot* memberships will be exchanged between nodes and eventually removed from the temporary set after multiple push. All nodes in the end should maintain the same copy of replicated membership data to reach eventual consistency, despite small chance of having residues.

## Assumptions

**No failure that creates a disconnected component of the network.**
- It's possible that when system encounter multiple node failures, new nodes joined before the restart could have recieved a incomplete or even empty membership data, thus entirely separating these nodes from the still running network and creating two disconnected components.
  
**Given node config is always assumed to be functioning for new node creation.**
- When a new node joins an existing network, it will be given a pair of [ip, port] of existing node of the network. This existing node is always assumed to be functioning and no failure or de-sync could happen to it throughout the whole creation procedural with the new node.

## Design

#### Data Structure
- **NodeConfig.class - `std::string ip, int port`**
  - this is used to facilitate the storage of node address
- **Membership.class - `int size, vector<NodeConfig>`**
  - Membership is the core of data exchange. It has a size and a vector of NodeConfig. Each node has a local copy of Membership `local_membership` and a vector `hot_rumors` as temporary holder for rumor mongering.
  - When nodes receive memberships that contain members who are not in the `local_membership`, they are added to the local and also the `hot_rumors` so they can be spread to other nodes as well. `local_membership` is the one that reaches eventual consistency in the end.
  - Members in `hot_rumors` are randomly selected and kicked after each exchange so *old rumors* can stop being *hot* anymore.
- **Message.class - `int type, Membership membership`**
  - Membership data are exchanged through Messages. A Message can be a **push** or a **pull**.
  - **Pull** sends a Message to an existing node who replies with the entire `local_membership` data. It's created and used at the startup process so a new node can get the entire copy of membership from another node in order to know the current network.
  - **Push** sends a Message to multiple randomly-selected nodes from the membership. Push Message only includes the `hot_rumors` so the network load can be reduced. This is different from *anti-entropy* design where all messages would be push+pull of the entire `local_membership`.

#### Node
- **Active Thread**
  - Active thread does not change any membership except the first pull request at startup. Active thread only changes `hot_rumors`.
  - When a node is created without an existing node's address, it will treat itself as the starting point of the network graph. If there's one, it will send a pull message to the existing node and grab all the membership data from that node. It is assumed that the node of origin does not fail during this process and the membership data grabbed are not isolated or disconnected.
  - After the pull, the node will start spreading rumors to `n` randomly picked nodes that are not itself. Since this is *rumor mongering* design, it will only send push messages contain membership of `hot_rumors`. The thread does not wait for response from the receiver nodes.
  - After each push, the node will roll dice `k` for each rumor in `hot_rumors` and decide whether it should be kicked out of the set. There's always a chance that a hot rumor might never be successfully spread to other nodes before it was kicked but the probability is low.
  - The node sleeps for x seconds before starting another round of rumor mongering.
- **Passive Thread**
  - Passive thread add new `membership` to both `hot_rumors` and `local_membership`. Therefore, this tread is responsible for printing out current state of membership data after each update.
  - Whenever a node receives a socket, it will generate a passive thread to deal with the incoming messages. 
  - If it's a pull message, it merges the new node config with its `local_membership` and `hot_rumors` and sends the entire `local_membership` back to the sender.
  - If it's a push message, it merges the incoming rumors with its `local_membership`. If there are rumors that haven't been added to the `local_membership` before, it will also be added to node's own `hot_rumors`. Since the senders of push messages do not wait for a response, the connection closes and thread stops.

### Not Achived
  - Ideally, the received connections should be kept alive by the threads in case of further rumors from the same senders until the senders disconnect but due to time constraint, this function is not implemented.
  - Currently, there's no membership deletion protocol and thus all nodes will forever keep their `local_membership` and try to make connections even if the members are dead. A timeout deletion (i.e. after y seconds, so that the hot rumor has cooled down, of not responding, the node presume the members dead and remove them from membership) was planned during the design phase but not implemented.

## CMD
If a server is not given an existing ip and port, it will assume it's the first node in the network and create a separate graph.
`./server ip port [existing_ndoe_ip existing_node_port]`

At start the network is small and there won't be any difference on replication between the nodes because the rumor will be spread to 2 random ndoes. But when the network gets bigger (i.e. # of nodes > 5), when anotehr new node joins the network, it's membership will suddenly appear in the connecting node but not necessarily all nodes in the network due to the random spreading process. However, after a few rounds of spreading, all nodes in the network should have the new node's membership data.

## Evaluation
Due to crude implemenation, the Membership State is printed by the passive thread. Since passive thread deals with incoming message, a print only happens **after** the `local_membership` has been touched (i.e. requeseted by pull, inserted with new entry, or inserted and ignored with existing entry).

#### Eventual Consistency
First start up 3 nodes. These 3 nodes should reach same replication state in 1 iteration as the size of randomness is set to 2.
`./server 10.200.125.60 11110`
`./server 10.200.125.61 11111 10.200.125.60 11110`
`./server 10.200.125.62 11112 10.200.125.60 11110`

Membership state should be:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`

Then start up another 2 nodes. both should be able to grab the complete network.
`./server 10.200.125.63 11113 10.200.125.60 11110`
`./server 10.200.125.64 11114 10.200.125.62 11112`

Membership state for `60 63` should immediately be:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`
`ip 10.200.125.63 port 11113`

Membership state for `62 64` should immediately be:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`
`ip 10.200.125.64 port 11114`

**Internally**, Membership state for `61` should still be:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`

But the next print (since printing is done after receiving a message, which should be a push from other nodes), `61` should receive either `63` or `64` depends on who reaches first.

Due to random gossips, the final membership state might take a few gossips to reach:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`
`ip 10.200.125.63 port 11113`
`ip 10.200.125.64 port 11114`

Then start up another node, again this node should be able to immediate grab some of the network from `62`. But the rest of the nodes (that are not the given node) should have a higher probability of not getting this node with only a few iterations.
`./server 10.200.125.65 11115 10.200.125.64 11114`

But in the end, all nodes should have `Membership` state like this:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`
`ip 10.200.125.63 port 11113`
`ip 10.200.125.64 port 11114`
`ip 10.200.125.65 port 11115`

#### Node Failure 1
This time, immediately starup 5 ndoes, wait for concensus.
`./server 10.200.125.60 11110`
`./server 10.200.125.61 11111 10.200.125.60 11110`
`./server 10.200.125.62 11112 10.200.125.60 11110`
`./server 10.200.125.63 11113 10.200.125.60 11110`
`./server 10.200.125.64 11114 10.200.125.60 11110`

Then create another node. But before it sends out any rumors, shut it down (the thread sleeps for a constant interval before sending gossips so there's time to interprupt). `64` should immediately receive `65`. The rest of the network should still have this node in `local_membership` as well after a few iterations.
`./server 10.200.125.65 11115 10.200.125.64 11114`

Membership state for `64` should immediately be:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`
`ip 10.200.125.63 port 11113`
`ip 10.200.125.64 port 11114`
`ip 10.200.125.65 port 11115`

The rest of the nodes, however, remain without `65` for a while but eventually reaches `64` state as well.

Then restart this node and give any existing node. This node should still be able to reach concensus with the rest of the network.
`./server 10.200.125.65 11115 10.200.125.61 11111`

In the end, all nodes should have Membership state like this:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`
`ip 10.200.125.63 port 11113`
`ip 10.200.125.64 port 11114`
`ip 10.200.125.65 port 11115`

#### Node Failure 2
This time, immediately starup 3 ndoes, wait for concensus.
`./server 10.200.125.60 11110`
`./server 10.200.125.61 11111 10.200.125.60 11110`
`./server 10.200.125.62 11112 10.200.125.60 11110`

Then create another node. 
`./server 10.200.125.63 11113 10.200.125.60 11110`

But before it sends out any rumors, shut the **existing node** `60` down.

Do not restart `60`. Although `61 62` haven't received `63` info from `60`, `63` should be able to reach them directly from the membership sent by `60`.

In the end, all still-alive nodes (`61 62 63`) should have Membership state like this:
>`ip 10.200.125.60 port 11110`
`ip 10.200.125.61 port 11111`
`ip 10.200.125.62 port 11112`
`ip 10.200.125.63 port 11113`
