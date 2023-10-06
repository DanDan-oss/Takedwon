## 下载工具:

> 下载steamcmd: https://developer.valvesoftware.com/wiki/SteamCMD



## 搭建服务器

1. #### 切换路径并解压steamcmd文件夹

2. #### 在解压目录文件夹下创建 `1.bat` 文件, 并写入代码

   ```bash
   steamcmd +login {$username} {$password} +app_update 223350 validate +quit
   ```

3. #### 进入解压的那个目录` /steamcmd/steamapps/common/DayZServer/ `目录,创建 2.bat 并写入代码

   > 注意填写自己的路径: `cd **/steamcmd/steamapps/common/DayZServer`

   ```c
   @echo off
   cls
   
   set version=1.0
   set wat=Dayz SA
   
   
   title %wat% Watchdog
   
   
   cd C:\steamcmd\steamapps\common\DayZServer
   
   :watchdog
   echo (%time%) %wat% started.
   start "Dayz_SA" /wait /affinity FF /high "DayZServer_x64.exe" -config=serverDZ.cfg
   echo (%time%) %wat% closed or crashed, restarting.
   goto watchdog
   ```

4. 配置服务器参数

   > 编辑`/steamcmd/steamapps/common/DayZServer/serverDZ.cfg`文件
   
   ```c
   hostname = "EXAMPLE NAME";  // 服务器名称
   password = "";              // 服务器密码
   passwordAdmin = "";         // 服务器管理员密码
   enableWhitelist = 0;        // 启用/禁用白名单（值0-1）
   maxPlayers = 60;            // 最大玩家数量
   verifySignatures = 2;       // 对照.bisign文件验证.pbos。（仅支持2个）
   forceSameBuild = 1;         // 启用时，服务器将只允许连接到与服务器具有相同.exe版本的客户端（值0-1）
   disableVoN = 0;             // 启用/禁用网络语音（值0-1）
   vonCodecQuality = 20;        // 网络语音编解码器质量，越高越好（值0-30）
   disable3rdPerson=0;         // 切换玩家的第三人称视图（值0-1）
   disableCrosshair=0;         // 切换十字线（值0-1）
   disablePersonalLight = 1;   // 对连接到服务器的所有客户端禁用个人指示灯
   lightingConfig = 0;         //0表示夜光设置，1表示夜光设置
   serverTime="SystemTime";    // 服务器的初始游戏时间。““系统时间”是指机器的本地时间。另一种可能是将时间设置为“YYYY/MM/DD/HH/MM”格式的某个值，例如“2015/4/8/17/23”。
   serverTimeAcceleration=12;  // 加速时间（值0-24）//这是游戏时间的时间乘数。在这种情况下，时间的移动速度会比正常情况快24倍，因此一整天只需一小时。
   serverNightTimeAcceleration=1;  //加速接近时间-数值为乘数（0.1-64），也乘以服务器时间加速值。因此，如果将其设置为4，将serverTimeAcceleration设置为2，则夜间的移动速度将比正常情况快8倍。三个小时后就过了一整晚。
   serverTimePersistent=0;     // Persistent Time（value 0-1）／将实际的服务器时间保存到存储器中，因此当处于活动状态时，下一次服务器启动将使用保存的时间值。
   guaranteedUpdates=1;        // 与游戏服务器一起使用的通信协议（仅使用数字1）
   loginQueueConcurrentPlayers=5;  // 在登录过程中并发处理的玩家数。当很多人同时连接时，应该防止连接期间性能的大幅度下降。
   loginQueueMaxPlayers=500;       // 可在登录队列中等待的最大玩家数
   instanceId = 1;             // DayZ服务器实例id，用于标识每个框的实例数及其包含持久性文件的存储文件夹
   storeHouseStateDisabled = false;// 禁用houses/doors持久性（值true/false），在持久性出现问题时可用
   storageAutoFix = 1;         // 检查持久性文件是否损坏，并用空文件替换损坏的文件（值0-1）
   class Missions
   {
       class DayZ
       {
           template="dayzOffline.chernarusplus"; // 在服务器启动时加载的任务。<MissionName><terranname>
   					      // 香草任务：Dayzoffline.Chernarusplus
   					      // DLC任务：dayzOffline.enoch
       };
   };   
   ```
   
5. 服务器全局变量,道具,车载,武器,事件,资源等等设置

   > 当搭建好服务器后,第一次启动会自动生成`/steamcmd/steamapps/common/DayZServer/mpmissions/dayzOffline.chernarusplus/db`路径
   >
   > `economy.xml` = [***初始设定: 负重, 重生, 保存***]()
   >
   > `events.xml` = ***[事件: 物资最大数量, 最小数量, 物资类型]()***
   >
   > `globals.xml` = ***[全局变量: 数量, 值]()***
   >
   > `types.xml` = ***[类型: 所有道具, 枪支, 装备, 车载, 飞机, 坦克, 大炮......]()***

   ```c
   // economy.xml = 初始设定: 负重, 重生, 保存
   
   // init = 初始化	load = 负载	respawn = 重生	save = 保存
   <?xml version="1.0" encoding="UTF-8" standalone="yes"?>
   <economy>
       <dynamic init="1" load="1" respawn="1" save="1"/>		// 动态
       <animals init="1" load="0" respawn="1" save="0"/>		// 动物
       <zombies init="1" load="0" respawn="1" save="0"/>		// 僵尸
       <vehicles init="1" load="1" respawn="1" save="1"/>		// 车辆
       <randoms init="0" load="0" respawn="1" save="0"/>		// 随机性
       <custom init="0" load="0" respawn="0" save="0"/>		// 习俗
       <building init="0" load="0" respawn="0" save="0"/>		// 建筑
       <player init="1" load="1" respawn="1" save="1"/>		// 玩家
   </economy>
   
   ```

   ```c
   //events.xml = 事件: 物资最大数量, 最小数量, 物资类型
   
   <events>
       <event name="AmbientHen">
           <nominal>3</nominal>
           <min>0</min>
           <max>50</max>
           <lifetime>33</lifetime>
           <restock>15</restock>
           <saferadius>40</saferadius>
           <distanceradius>0</distanceradius>
           <cleanupradius>0</cleanupradius>
           <flags deletable="0" init_random="0" remove_damaged="0"/>
           <position>fixed</position>
           <limit>mixed</limit>
           <active>1</active>
           <children>
               <child lootmax="0" lootmin="0" max="2" min="1" type="Animal_GallusGallusDomesticus"/>
               <child lootmax="0" lootmin="0" max="4" min="1" type="Animal_GallusGallusDomesticusF_Brown"/>
               <child lootmax="0" lootmin="0" max="4" min="1" type="Animal_GallusGallusDomesticusF_Spotted"/>
               <child lootmax="0" lootmin="0" max="4" min="1" type="Animal_GallusGallusDomesticusF_White"/>
           </children>
       </event>
   /* <child lootmax="0" lootmin="0" max="2" min="1" type="Animal_GallusGallusDomesticus"/>
   这种子标签 里面配置了各个地区刷新的物资等数量参数， lootmax 是物资最大数量 lootmin是物资最小数量 max是最大 min是最小type是类型,物资类型 */
   ```

   ```c
   // globals.xml = 全局变量: 数量, 值
   
   <variables>
       <var name="AnimalMaxCount" type="0" value="200"/>				// 动物最大数量
       <var name="CleanupAvoidance" type="0" value="100"/>				// 避免清理
       <var name="CleanupLifetimeDeadAnimal" type="0" value="1200"/>		// 清理动物
       <var name="CleanupLifetimeDeadInfected" type="0" value="330"/>
       <var name="CleanupLifetimeDeadPlayer" type="0" value="3600"/>
       <var name="CleanupLifetimeDefault" type="0" value="45"/>		// 刷新服务器时间的默认值
       <var name="CleanupLifetimeLimit" type="0" value="50"/>			// 清除时间限制
       <var name="CleanupLifetimeRuined" type="0" value="330"/>		// 清理垃圾销毁
       <var name="LootProxyPlacement" type="0" value="1"/>
       <var name="RespawnAttempt" type="0" value="2"/>					// 尝试重新创建
       <var name="RespawnLimit" type="0" value="20"/>					// 重新创造限制
       <var name="RespawnTypes" type="0" value="12"/>					// 重新创造类型
       <var name="RestartSpawn" type="0" value="100"/>					// 重新生成制造
       <var name="SpawnInitial" type="0" value="1200"/>				// 创造初始值
       <var name="TimeHopping" type="0" value="60"/>
       <var name="TimeLogin" type="0" value="15"/>
       <var name="TimeLogout" type="0" value="15"/>
       <var name="TimePenalty" type="0" value="20"/>
       <var name="ZombieMaxCount" type="0" value="1000"/>				// 最大僵尸数量
       <var name="ZoneSpawnDist" type="0" value="300"/>				// 制造区域生成
   </variables>
   ```

   ```c
   // types.xml = 类型: 所有道具, 枪支, 装备, 车载, 飞机, 坦克, 大炮......
   
   <type name="ACOGOptic">				// 物品名称
           <nominal>15</nominal>			
           <lifetime>7200</lifetime>		// 使用期限
           <restock>1800</restock>			// 存储
           <min>8</min>				// 最小容量
           <quantmin>-1</quantmin>			// 最小量
           <quantmax>-1</quantmax>			// 最大量
           <cost>100</cost>				// 价值
           <flags count_in_cargo="0" count_in_hoarder="0" count_in_map="1" count_in_player="0" crafted="0" deloot="0"/>    // 货物计数, 在存储室计数, 地图中计数, 玩家计数, 手工制作, 掠夺
           <category name="weapons"/>		// 类别
           <usage name="Military"/>		//	使用途径
       </type>
   ```

6. 新人出生装备

   > 编辑
   >
   > `/steamcmd/steamapps/common/DayZServer/mpmissions/dayzOffline.chernarusplus/init.c`

   ```c
   void main()
   {
   	//INIT WEATHER BEFORE ECONOMY INIT------------------------
   	Weather weather = g_Game.GetWeather();
   
   	weather.MissionWeather(false);    // false = use weather controller from Weather.c
   
   	weather.GetOvercast().Set( Math.RandomFloatInclusive(0.4, 0.6), 1, 0);
   	weather.GetRain().Set( 0, 0, 1);
   	weather.GetFog().Set( Math.RandomFloatInclusive(0.05, 0.1), 1, 0);
   
   	//INIT ECONOMY--------------------------------------
   	Hive ce = CreateHive();
   	if ( ce )
   		ce.InitOffline();
   
   	//DATE RESET AFTER ECONOMY INIT-------------------------
   	int year, month, day, hour, minute;
   	int reset_month = 9, reset_day = 20;
   	GetGame().GetWorld().GetDate(year, month, day, hour, minute);
   
   	if ((month == reset_month) && (day < reset_day))
   	{
   		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
   	}
   	else
   	{
   		if ((month == reset_month + 1) && (day > reset_day))
   		{
   			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
   		}
   		else
   		{
   			if ((month < reset_month) || (month > reset_month + 1))
   			{
   				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
   			}
   		}
   	}
   }
   
   class CustomMission: MissionServer
   {
   	void SetRandomHealth(EntityAI itemEnt)
   	{
   		if ( itemEnt )
   		{
   			int rndHlt = Math.RandomInt(55,100);
   			itemEnt.SetHealth("","",rndHlt);
   		}
   	}
   
   	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
   	{
   		Entity playerEnt;
   		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
   		Class.CastTo(m_player, playerEnt);
   
   		GetGame().SelectPlayer(identity, m_player);
   
   		return m_player;
   	}
   
       // start 人物出生装备初始化函数
   override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
   	{
   	EntityAI itemEnt;
   	ItemBase itemBs;
   	float rand;
                   
   		itemEnt = player.GetInventory().CreateInInventory("Mag_STANAG_30rND");
   		if ( Class.CastTo(itemBs, itemEnt ) )
    		itemBs.SetQuantity(30);	// 子弹数量
   
    		SetRandomHealth(itemEnt);  // 装备品质
   
    		itemEnt = player.GetInventory().CreateInInventory("TTSKOPants");
    		SetRandomHealth(itemEnt);
   
    		itemEnt = player.GetInventory().CreateInInventory("TTSKOJacket_Camo");
    		SetRandomHealth(itemEnt);
   
    		itemEnt = player.GetInventory().CreateInInventory("AliceBag_Black");
    		SetRandomHealth(itemEnt);
   
    		itemEnt = player.GetInventory().CreateInInventory("M4A1");
    		SetRandomHealth(itemEnt);
                           
    		rand = Math.RandomFloatInclusive(0.0, 1.0);
    		if ( rand < 0.35 )
   			itemEnt = player.GetInventory().CreateInInventory("Apple");
    		else if ( rand > 0.65 )
   			itemEnt = player.GetInventory().CreateInInventory("Pear");
    		else
   			itemEnt = player.GetInventory().CreateInInventory("Plum");
                   
    		SetRandomHealth(itemEnt);
   
           }
   };
   //end
   
   Mission CreateCustomMission(string path)
   {
   	return new CustomMission();
   }
   ```

   

7. asd

