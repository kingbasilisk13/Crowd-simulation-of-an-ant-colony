# Crowd simulation of an ant colony.


## Abstract:
Machine learning and AI enable us to program decision-making for non-player characters (NPCs) in video games. They can also be used to simulate the behavior of real-world animals. In the following research project, I will attempt to simulate the workings of an ant colony using crowd simulation. I will use a combination of steering behaviors, decision-making, and influence maps to create the simulation model.


## Introduction:
Did you know that ants can be found nearly everywhere on earth, except Antarctica and a few inhospitable islands. This fact clearly indicates that ants are adapt at spreading to new places and surviving there. While this phenomenon poses challenges to the local ecosystems disrupted by the ants, it falls outside the scope of this project.  
The point is that ants display remarkable teamwork. Their nests consist of multiple chambers, each serving their own purpose: A chamber where the queen resides, brood chambers ordered by the age of the brood, and garbage sites for disposing of leftover food and dead ants. These garbage sites are created to prevent the smell from attracting predators and to prevent the growth of mold within the nest.  
Ants also have the ability to store food. To achieve this, ants make use of a special stomach called a social stomachs. They then use a process called trophallaxis to regurgitate food from one ant to another. They accomplish all of this and more while being nearly blind. Instead of relying on sight and sound to convey meaning to one another, they use specialized pheromones. There are around 10 to 20 pheromones that ants use, each with its own meaning. For example: to indicate where an ant found food, they lay down a pheromone trail. They also use this trail to find their way back home. When an ant dies, they start releasing a pheromone that alerts nearby ants of the dead ant. The colony then collects the body and brings it to the garbage site.  
This is only a general description, and there are hundreds of ant species, each with its own unique quirks. For example, there are leaf cutter ants that make their nests in trees and Dracula ants that survive by drinking a small amount of blood from their own brood. For this project, I will limit myself to the following behaviors.

1. **Queen**: The queen ant is a stationary ant that produces new ants every X seconds. She cannot get food herself and thus will need to be fed by worker ants. She is the center of the nest and has the most influence over the other ants.
2. **Worker ants**: These ants are the most common and do most of the work. They go out and forage for food, collecting it in their social stomach. They feed the queen and other ants. Worker ants have a limited lifespan and will die. They move dead ants to the garbage site.
3. **Soldier ants**: Larger than worker ants, these ants are tasked with protecting the colony. They react to external threats and intruders, defending the colony.

To achieve these behaviors, the following components will be used to create a crowd simulation model:
* **Steering behaviors**: This provides a way to control how the ants move.
* **Decision making**: This allows the ants to decide what actions to take based on their current situation.
* **Influence map**: This serves as a substitute for pheromones, enabling the ants to map out their surroundings without the need for pathfinding.

The project will make use of a framework provided by the teachers of gameplay programming at Digital Arts & Entertainment (DAE). This framework already contains working code for the previous components, which I will use as a base for the project.


## Crowd Simulation
Crowd simulation is the process of simulation the movement and/or dynamics of a large number of entities. It is most commonly used to create realistic scenes of large crowds in movies and video games. It can also be used in crisis training, architecture and urban planning, and evacuation simulation.  
In all technicality crowd simulation focuses primarily on simulating human-like entities. Because this project is trying to simulate ants it is technicaly not seen as crowd simulation. One could argue that the decision making is specific enough that it could be considerd crowd simulation.

There are several approaches to crowd simulation, each with there own advantages and disadvantages based on the following two factors: crowd size and time scale.
* **Crowd size**: this refers to the number of entities in the simulation. There is no exact number to specify if a crowd is large or small so it depends on the researcher and the strength of the used machine.
* **Time scale**: this refers to how the objective of the simulation effects the length of the simulation. For example the simulation of how ideologies spread amongst a population will run for a long time as the real events can take months or even years.

These 2 factors are used to classify the folowing crowd simulations.
* **Flow-based Approach**: a simulation focused on a crowd as a whole. Individuals have no distinct behavior that occurs due to input from their surroundings. Mostly used to simulate the flow of movement. **Crowd size**: large, **Time Scale**: short.
* **Entity-based Approach**: a simulation that implements a set of physical, predefined, and global laws. The entities do not think for themselves and their movement and actions are determined by the global laws. This model is most often used to research crowd dynamics such as jamming and flocking. **Crowd Size**: Small to medium, **Time Scale**: short.
* **Agent-based Approach**: Each agent in this simulation has a degree of intelligence. They can each react to each situation on their own based on a set of decision rules. They also use information from there surrounding to decide how to react. This is most often used to simulate realistic crowds. Crowd size and time scale depend entirely on what the researcher is trying to simulate.

This project falls somwhere between entity-based and agent-based approach. All ants will folow a predefined global law but they can also make decisions individualy based upon there suroundings. But we still need a way to say what actions they can take and which one to take.

### Rule-based AI
Rule-based AI is a way to implement decision-making in a crowd simulation. In this AI, each agent follows a script: when X occurs, do Y. This is used in cases where different agents with different roles are required. This AI usually uses a hierarchy, for example, Maslow's hierarchy of needs, where the lower the need is, the stronger it is.

![By FireflySixtySeven - Own work using Inkscape, based on Maslow's paper, A Theory of Human Motivation., CC BY-SA 4.0, https://commons.wikimedia.org/w/index.php?curid=36551248](https://upload.wikimedia.org/wikipedia/commons/3/33/MaslowsHierarchyOfNeeds.svg)
By FireflySixtySeven - Own work using Inkscape, based on Maslow's paper, A Theory of Human Motivation., CC BY-SA 4.0, https://commons.wikimedia.org/w/index.php?curid=36551248

An example of this is a student walking to class. An explosion happens in front of them, and the student runs away. The student's need for safety is much stronger than its need to go to class. In the case of this project, the three kinds of ants will each have their own needs and will act according to which need is the strongest at that moment. Here is a rough description of the needs of each type of ant going from strongest to weakest.

Queen:
1. Call for help when attacked.
2. Do not starve.
3. Spawn more brood

Soldier:
1. Protect the colony.
2. Do not starve.

Worker:
1. Feed the colony.
2. Clean up garbage.
3. Collect food.
4. Do not starve.

In actual ant hives, these tasks would be split among the ants, with the age of the ant deciding what it must do. Younger ants stay and take care of the hive, while older ants go out looking for food. Therefore, this is not accurate to real-life ants, but it is the order of needs that this project will use.

## Steering behaviors:
Steering behaviors are an amalgamation of different behaviors used to manage the movement of an AI. These are often very simple behaviors that utilize external factors such as goals, other agents, and obstacles to move the agent. Here are a few examples:

* **Seek**: Move to a specific location.
* **Flee**: Move away from a specific location.
* **Pursuit**: Predict the location where another moving agent will be in the near future and move to that location.
* **Evade**: The opposite of pursuit. Move away from the predicted location of another moving agent.
* **Wander**: Choose a random location in front of the agent and move towards it.

On their own, these behaviors are very limited, but combining them opens up a whole new world of possibilities. For this project I have chosen to use Blended steering.

### Blended steering:
Blended steering assigns a certain weight to each steering behavior. The higher the weight of the behavior, the more influence it will have on the movement of the agent. For example, if you combine the wander behavior and the seek behavior, each with a weight of 50%, you will get "drunk seek": an agent that tries to reach a specific point while never following a straight path.

add image


## Decision making:
An agent should have the ability to choose what to do in certain situations. This is mostly manually programmed, but doing this for every situation is hard and time-consuming. It also results in code that contains giant if-else statements. To avoid this, several ways of decision-making have been developed to simplify and clean up this process. For this project I have chosen to use Behavior Trees.

### Behavior tree:
A behavior tree is a directed acyclic graph, a directed graph with no cycles. The program always starts at the same point, the root, and then makes its way down the tree. Depending on the results of the nodes, the agent will perform certain actions, or the program will return back down the tree. The nodes that return a result are called **action nodes** and can return one of the following three actions: succeeded, failed, or running. This result is sent to the **composite node**. This node decides what to do depending on the result of the action node. For this project I have chosen to use only the following two composite nodes: the sequence node and the selector node.

#### The sequence node:
The sequence node attempts to execute each action node in the given order. Depending on the results of the action nodes, the sequence executes a specific action:
* Action node returns success: The sequence node starts executing the next action node.
* Action node returns running: The sequence node aborts the execution and returns running.
* Action node returns failed: The sequence node aborts the execution and returns failed.
You can compare this node to an AND operation.

add image

#### The selector node:
The selector node attempts to execute each action node until a certain result is received:
* Action node returns success: The selector node stops and returns success.
* Action node returns running: The selector node stops and returns running.
* Action node returns failed: The selector node starts executing the next child.
Just like the sequence node, you can compare this node to the OR operation.

add image

Action nodes are used to make the agent do something and cannot have child nodes. This can be very limiting, especially if in a sequence you want to check if a certain condition is met before executing the rest of the sequence. To solve this, we use Conditional nodes. Conditional nodes are nodes used to check a condition, and they return either true or false.


## Influence maps
Influence maps are a collection of the influence that agents have on the world. It is a spatial storage of influence that agents can use to make decisions. For example, if a lot of enemy agents have walked through the same area, their influence on that area will be significant, and an agent may know not to go there. This helps a group of agents make decisions more strategically. Different information needs to be stored in separate influence maps. Influence maps can also be used to store information from the past, predict agents' future positions, and give agents imperfect information. Influence spreads over the map; we call this influence propagation. There are multiple ways to propagate, but for this project I will use a combination of the following 2: convolution filters and map flooding.

### Convolution filters:
A convolution filter is a blurring technique. Using a small matrix of numbers, also known as a kernel, on each pixel of an image will result in a new image where each pixel is a weighted sum of the pixels in the original image. Using this on an influence map means that each tile on the map will hold the weighted influence of the tiles in the previous iteration.

* The advantage of this is that each tile gets processed only once per iteration.
* The disadvantage of this is that it cannot combine influences into a new higher influence. It also does not work well when there are only a small number of influence sources.

### Map flooding:
Map flooding uses the flood fill algorithm to spread influence across the map. The flood fill algorithm is an algorithm that determines and alters the area connected to a given node. An example of this is the bucket tool in MS Paint.  
* The advantage of this is that it is fast and can use terrain analysis to avoid obstacles.
* The disadvantage of this is that it is not good at combining influences because it picks the highest one.

Combining these two will result in influence propagation that will take the highest influence value of all neighbors and interpolate with the current influence. To control this, two parameters are used: Decay and Momentum.
* Momentum: Indicates how difficult it is to modify the value of something. For example, a car with high momentum will not stop easily. Higher momentum will result in slower propagation.
* Decay: How much influence decreases over distance. Lower decay will result in further spread of the influence.


## Creating the project
### The ants
There are 3 types of ants: Queen, SoldierAnt and WorkerAnt. They are all derived from the base class AntBase. This base class contains all the code that every ant type need. This includes: accessing to the private variables, update function that updates the influence maps and the movement behavior. The base class also contains a pointer to the map where the ant needs to read from and a list of pointers to maps where it needs to write to. This is because there are situations where ants have influence on multiple maps at once.

### Blended steering
Each ant classes uses blended staaring object with a seek and a wander steering. The wander behavior is used to force the ant to explore. The seek behavior is used to move the ant in the direction of the highest value of the influence map.

### Influence maps
In total there are 5 diferent influence maps.  
1. **FoodMap**: this map is used to indicate where food sources can be found.
2. **HomeMap**: this map is used to indicate where the queen and by extend the nest can be found.
3. **HungerMap**: this map is used to indicate where ants who need food are.
4. **DeathMap**: this map is used to indicate where dead ants are.
5. **ThreatMap**: this map is unused, but was suposed to be used ti indicate where invaders are.


### Behavior Tree
This is by far the most import and complicated part of the project. There are 4 main branches:
1. DeadAntSequence
2. WorkerAntSequence
3. SoldierAntSequence
4. QueenAntSequence

I will give a short description of how each branch works, going into more detail on the more interesting parts.

**QueenAntSequence**:
1. Give the queen the pointer to the home map.
2. Check if the queen can spawn brood. If so then create either a new soldier or a new worker and add them to the list. If not continue.
3. Check if the queen needs food. If so then give her a pointer to the hunger map. If not continue.

**SoldierAntSequence**:
1. Set the home map as the read map for the soldier. There only purpose is to guard the nest.
2. Check if they are hungry, same as with the queen.

**DeadAntSequence**:
1. Check if the ant is dead.
2. Give the dead ant a pointer to the dead map to write to.
3. Make shure the map the ant reads from is null.
4. Chek if the dead ant is a worker ant. If so check if it was carying another dead ant. If it was it will drop that dead ant.

**WorkerAntSequence**
1. Chek if the ant is idle. If so it will be given the job to either scavange for food or clean up dead ants. Depending on how many ants do wat job they are assigned a job to make shure there is a balance.
2. Check if the ant is scavanging. If so it will get the food map to read from and the home map to write to.
2.1. Check if the ants social stomach is full. If that is the case it will start feeding the other ants.
2.2. Check if the ant is near food. If that is the case it will: go to the food, stop writing to the home map and start writing to the food map, stop reading from any map, collect food.
3. Chek if the ant is feeding.
3.1. Check if its social stomach is empty. If so the ant will go idle.
3.2. Start reading from the hunger map and stop writing to any map. It will also check if it can sense any hungry ants. To do this it will check if the value it gets from the read map is above a 0.
3.2.1. If it is near a hungry ant it will set that ant as its target and start feeding it.
3.3. If it is not near a hungry ant it will return to the nest by setting the home map as the read map.
4. Check if the ant is cleaning
4.1. If the ant is holding a dead ant it will set the garbage site as its target and move there.
4.1.1 If it is near the garbage site it will drop the dead ant and become idle.
4.2. The ant will set the dead map as its read map and check if there are any dead ants nearby.
4.2.1 If there is a dead ant nearby that is not being caried yet it will collect the dead ant.
4.3. If it does not detect any dead ants it will set the home map as the read map.
5. Finaly there is a check to see if the ant is hungry. Same as with the soldier and queen.


# Results
The created simulation is able to run for a rather long time. Soldier ants constantly protect the queen. Worker ant clean up dead ants ant collect food to feed the colony. The queen ant constantly spawns new brood. The trails on the influence maps do not stay long and are prone to errors, ant looping back on themselfs.



# Conclusion
The project can run for a rather long time, I am unshure how long. Some parts where not implemented, for example outside threats, and there is still a lot of room for improvement. The diferent values used for the influence map could be tweaked more or even changed dynamicaly depending on the situation. The behavior of the ants could be expanded upon or maybe even be made to use something else then a behavior tree. Something that would allow the ants to chose what to do depending on the influence maps. The influence maps could be combined into 1 or more maps where each could influence the value of the other.  
Making this project I realised that creating a crowed simulation is much more then just combining movement behavior and decision making. You have to plan ahead how the diferent agents are going to interect whith eachother. How they interact with the world and how the world influences them. If I where to continue working on this project I would do more extensive research about crowd simulations and influence maps. I would also experement with changing the values of variables like momentum and decay dynamicaly while the simulation is running.





# Sources
Crowd simulation Wikipedia: [https://en.wikipedia.org/wiki/Crowd_simulation](https://en.wikipedia.org/wiki/Crowd_simulation)
Steering behaviors: Seeking and Arriving: [https://www.gamedev.net/blogs/entry/2264855-steering-behaviors-seeking-and-arriving/](https://www.gamedev.net/blogs/entry/2264855-steering-behaviors-seeking-and-arriving/)
The Core Mechanics of Influence Mapping by Alex J. Champandard: [https://www.gamedev.net/tutorials/programming/artificial-intelligence/the-core-mechanics-of-influence-mapping-r2799/](https://www.gamedev.net/tutorials/programming/artificial-intelligence/the-core-mechanics-of-influence-mapping-r2799/)


Image Processing: Convolution filters and Calculation of image gradients by Abhishake Yadav: [https://www.linkedin.com/pulse/image-processing-convolution-filters-calculation-gradients-yadav](https://www.linkedin.com/pulse/image-processing-convolution-filters-calculation-gradients-yadav)
Coding Adventure: Ant and Slime Simulations by Sebastian Lague: [https://www.youtube.com/watch?v=X-iSQQgOd1A](https://www.youtube.com/watch?v=X-iSQQgOd1A)
AntsCanada, youtube channel that is main source of information about ants: [https://www.youtube.com/@AntsCanada](https://www.youtube.com/@AntsCanada)

