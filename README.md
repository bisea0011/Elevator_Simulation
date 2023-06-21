软件工程专业-西安电子科技大学计算机科学与技术学院 - by Bisea0011

***概述：一个使用C++语言和面向对象编程思想完成的电梯运行仿真程序，仿真了现实生活中电梯运行时电梯和乘客的行为，并设计了一些电梯运行情况的统计数据。***

###### *正文部分：设计思路、关键数据结构定义和类的接口定义、主要算法流程以及遇到的问题和解决方案：*

#### 第一次设计：

为实现电梯仿真模拟，初步设想是设置两种类，**电梯类和乘客类**，同时设计一个负责控制仿真流程、实现电梯分配、信息输出、数据统计的**main函数**；

**仿真的思路**是，通过楼层参数的加减来模拟电梯的上下运行以及乘客的乘梯过程，所有电梯运行一次（上行/下行/闲置）为一轮仿真，**在每一轮的仿真中，执行的步骤如下：**

**1** 生成乘客：生成来到大楼一楼的乘客，将乘客信息添加到大楼乘客组；

**2** 电梯上下客：

> 所有电梯执行以下步骤：
>
> 1\) 比较当前到达的楼层与电梯目标楼层的关系，更新运行状态（上行/下行/闲置）
>
> 2\) 检索自己内部有无到达目标楼层的乘客，下客（电梯乘客组删去该乘客）

3\) 检索当前楼层有无被分配给自己的乘客，上客（电梯乘客组加入该乘客）

4\) 如果自己没有在闲置的时候被预定，遍历自己的乘客组的目标楼层，根据运行方向设置目标楼层为乘客目标楼层中的最高或最低楼层；如果自己仍然处于被预定的状态，也就是还没有接到预定自己的乘客，不改变当前目标楼层（预定自己的乘客所在楼层）

> 5\) 比较当前到达的楼层与目标楼层的关系，更新运行状态（上行/下行/闲置）

**3** 分配电梯：遍历大楼的乘客当前状态，为有乘梯请求的乘客（目标楼层和当前楼层不一致和到了目标楼层但是已经停留足够长时间的乘客）分配电梯，

> 即遍历当前电梯：
>
> a.没有被预定的、运行方向一致的、可以接到该乘客的电梯；
>
> b.闲置的电梯；
>
> c.被预定的、运行方向一致的、可以接到该乘客的，同时递送该乘客不影响自己的预定者的行程的电梯——满足这三种情况一种的电梯被分配给该乘客（分配方式是将该电梯编号赋给该乘客）

**4** 电梯运行

**5** 输出该轮的仿真信息：

> 仿真信息的设计是：当前时间，遍历电梯，输出运行状态、所在楼层、是否被预定、乘客组信息（乘客数量，电梯内的每位乘客的编号与目标楼层）

**-------如此循环直到判断所有乘客已到达目的地，跳出循环结束仿真**。

其他的在设计方面的可说的点：

电梯上下客时及时地对乘客对象的状态进行变更；

每次乘客上下电梯的时候均有该动作消耗时间在当前时间的累加；

每位乘客预定了一部闲置电梯之后不可再预定其他电梯；

#### 第二次设计：

相对第一次设计的变更：

**1** 在生成每位乘客时设置其乘梯次数为1-10的随机数；

> **2** 给乘客添加属性：在当前楼层的停留时间和下次乘梯时刻，每次出电梯时随机设置停留时间为10-120秒之间的随机数，并且计算乘客下次乘坐电梯的时刻，之后通过在分配电梯时的当前时刻与该时刻的比较来判断乘客在当前楼层的停留是否结束，并决定是否给乘客分配电梯；
>
> **3** 判断乘客停留结束之后，首先更新乘客的目标楼层（一个不同于当前楼层的目标楼层，如果检测到是最后一次乘梯（也就是返程），会将目标楼层设置为1 ），再给乘客分配合适的电梯；
>
> **4** 检索到已经结束乘梯的乘客会将他们请出大楼（大楼乘客组删去该乘客）
>
> 当其离开大楼时时会显示提示信息 "Passenger ID finished his(or her) trips!"
>
> **5** 检测到大楼已无乘客（乘客组为空），即所有乘客均已结束乘梯活动离开了大楼的时候，结束仿真。

#### 第三次设计：

相对第二次设计的变更：

> 分配电梯时先按照之前的分配规则，给乘客预分配电梯编号，之后添加了判断函数，将预分配的电梯编号和乘客的目标楼层作为参数输入，如果判断乘客的目标楼层是该编号的电梯所可以到达的，即返回真，然后将该电梯正式分配给该乘客，不然换另一部电梯进行判断。以这种方式实现对电梯运行规则的修改

#### 第四次设计：

相对第三次设计的变更：

> 1 电梯运行情况的统计：给电梯对象添加两个属性：运行时间与闲置时间，每轮给电梯运行时，如果电梯是上下行，则运行时间增加（也就是电梯移动一层的时间），如果电梯是闲置，则闲置时间增加；在电梯上下客时，每上/下一位乘客，电梯的运行时间增加（也就是一位乘客上/下的时间）。在仿真结束后显示每部电梯在本次仿真过程中的运行时间和闲置时间。
>
> 2 乘客等待时间的统计：我选择去统计的是在本次仿真过程中平均每位乘客一次请求的等待所需要的时间，在每轮电梯运行中，当检测到发出请求的乘客仍未进入电梯，则等待时间增加。最终在仿真结束后计算平均每位乘客一次请求的等待所需要的时间。

#### 第五次设计：

相对第四次设计的变更：

> 为了达到预警高峰期的目的，我的想法是通过选取上一次仿真过程中大楼各电梯繁忙程度最高的前20个时段来预测下次仿真时电梯的使用的高峰期：
>
> 通过在main函数新定义一个类DataOfCurrentTime来定义一种记录当前时刻电梯运行繁忙程度的对象，包含currentTime（当前时刻）和busyLevel（繁忙程度）两个数据成员（这里直接用当前时刻上下行的电梯的数量作为大楼电梯总体的繁忙程度），在每轮运行的最后，遍历各电梯当前的状态，如果是上下行则busyLevel++，遍历完所有电梯后创建DataOfCurrentTime对象并存入operationCount（时序记录表）
>
> 当本次仿真结束后，从时序记录表中遍历筛选出busyLevel最高的20个时刻作为峰值开始时刻，之后输出预警信息。

#### 运行结果展示

这里给出第五次设计之后较为完备的电梯仿真程序的运行结果截图，并附有相应的解释：

![image](\Screenshot_of_code_running\图片1.jpg)

![image](\Screenshot_of_code_running\图片2.jpg)

![image](\Screenshot_of_code_running\图片3.jpg)

![image](\Screenshot_of_code_running\图片4.jpg)

![image](\Screenshot_of_code_running\图片5.jpg)

![image](\Screenshot_of_code_running\图片6.jpg)

![image](\Screenshot_of_code_running\图片7.jpg)

![image](\Screenshot_of_code_running\图片8.jpg)

——图片阅读顺序为从上到下，

这里选取Passenger 3作为观察对象进行乘梯过程的跟踪，

可以看到Passenger 3首先是在2792秒时下到了目标楼层6楼，出了电梯，

之后在仿真时刻3100秒之前结束了楼层停留，申请乘电梯并被分配了Elevator 2，

3100秒时Elevator 2到达6楼接到了Passenger 3，之后一路下行，

在3123秒时Passenger 3 下到了1层，结束乘梯，离开大楼。

下边是仿真结束时的统计信息显示：

![image](\Screenshot_of_code_running\图片9.jpg)

——首先显示的是67位乘客在前100分钟陆续到达大楼，并于5966秒结束乘梯，

然后是电梯总运行时间和总空闲时间统计，平均一位乘客一次请求所花费的时间统计

![image](\Screenshot_of_code_running\图片10.jpg)

——这里是运行的峰值时段的统计结果输出显示

#### **设计总结**

本次设计让我切实感受到了面向对象程序设计思想的内涵与意义。

类设计，数据结构设计，方法设计，控制条件设计，这些都是对真实世界的建模，

我感触很深的一点是，要想写好一个面向对象程序，搭建一个有秩序的稳定运行的系统，

我需要去深刻且全面地理解和抽象模拟的对象和环境的关键属性、关键行为，

在这次电梯仿真程序的设计之中，我就能明显感受到，我需要将整个电梯运行过程的步骤给严谨细致的拆解、简化、等效转换，从而实现数据的变化对此真实过程的反映。

Debug的过程很多，但也颇有收获，自己对于如何设置print语句来发现问题所在，看见bug应该往哪些可能的原因去想，这些能力都有所提升；

自己在本次电梯仿真程序的设计中确实想出了一些行之有效的方法，模拟的效果尚可，但也仍然存在一些瑕疵，待暑假有时间，我愿意去将它继续优化，去实现更多的想法。



***//代码中有一些用于debug的语句我没有去去除，而是直接注释处理***

***//使用的是C++11的语法标准***
