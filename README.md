<h2>ZombieScenario - BOT 攻击 NPC 丧尸 AI 插件</h2>

<p>一个可配置的扩展插件，用于 <strong>CS 起源 ZombieScenario</strong> 模式，允许 <strong>BOT 主动攻击 NPC 丧尸并具备智能 AI 行为</strong>。</p>

<h3>📦 插件依赖</h3>

<ul>
  <li>编译时必须包含 ZombieScenario API：</li>
</ul>

<pre><code>#include &lt;HanZombieScenarioAPI&gt;
</code></pre>

<ul>
  <li>依赖文件：HanZombieScenarioAPI.inc</li>
  <li><strong>必须与 ZombieScenario 主插件同时使用</strong></li>
</ul>

<h3>⚙️ 配置项说明</h3>

<ul>
  <li><strong>enable_plugins</strong><br>
  是否启用 BOT 攻击 NPC AI 系统。<br>
  1 = 启用，0 = 禁用<br>
  <strong>默认值：1</strong></li>

  <li><strong>Bot_CanSee_Distance</strong><br>
  BOT 能看到目标的最大距离。<br>
  0.0 = 无限<br>
  <strong>默认值：0.0</strong></li>

  <li><strong>Bot_Attack_Cooldown</strong><br>
  BOT 攻击间隔（秒），最小为 0.1。<br>
  <strong>默认值：0.1</strong></li>

  <li><strong>Bot_StaySafe_Distance</strong><br>
  BOT 是否启用与目标保持安全距离。<br>
  1 = 启用，0 = 禁用<br>
  <strong>默认值：1</strong></li>

  <li><strong>Safe_Distance</strong><br>
  与目标小于该距离时后退。<br>
  <strong>默认值：300.0</strong></li>

  <li><strong>FallBack_Speed</strong><br>
  BOT 后退速度。<br>
  <strong>默认值：220.0</strong></li>

  <li><strong>Bot_ForwardTo_Distance</strong><br>
  BOT 是否启用前进追击远程目标。<br>
  1 = 启用，0 = 禁用<br>
  <strong>默认值：1</strong></li>

  <li><strong>Forward_Distance</strong><br>
  与目标大于该距离时前进。<br>
  <strong>默认值：400.0</strong></li>

  <li><strong>Forward_Speed</strong><br>
  前进速度。<br>
  <strong>默认值：220.0</strong></li>

  <li><strong>Block_BotUse_Grenade</strong><br>
  是否禁用 BOT 使用手雷（防止 CS:S BOT 捏雷不扔）。<br>
  1 = 禁用，0 = 允许<br>
  <strong>默认值：1</strong></li>
</ul>

<hr>

<h2>ZombieScenario - BOT Attack NPC Zombie AI</h2>

<p>A configurable extension plugin for <strong>CS:S ZombieScenario</strong>, allowing <strong>BOTs to actively attack NPC zombies with intelligent AI behavior</strong>.</p>

<h3>📦 Requirements</h3>

<ul>
  <li>Must include the ZombieScenario API in your code:</li>
</ul>

<pre><code>#include &lt;HanZombieScenarioAPI&gt;
</code></pre>

<ul>
  <li>Requires file: HanZombieScenarioAPI.inc</li>
  <li><strong>Must be used alongside the main ZombieScenario plugin</strong></li>
</ul>

<h3>⚙️ Configuration Variables</h3>

<ul>
  <li><strong>enable_plugins</strong><br>
  Enable or disable the BOT-to-NPC AI system.<br>
  1 = Enabled, 0 = Disabled<br>
  <strong>Default: 1</strong></li>

  <li><strong>Bot_CanSee_Distance</strong><br>
  Maximum distance BOTs can detect targets.<br>
  0.0 = Unlimited<br>
  <strong>Default: 0.0</strong></li>

  <li><strong>Bot_Attack_Cooldown</strong><br>
  Minimum interval between BOT attacks (in seconds).<br>
  <strong>Default: 0.1</strong></li>

  <li><strong>Bot_StaySafe_Distance</strong><br>
  Enable BOTs to keep a safe distance and move backward.<br>
  1 = Enabled, 0 = Disabled<br>
  <strong>Default: 1</strong></li>

  <li><strong>Safe_Distance</strong><br>
  BOTs will move back if closer than this distance.<br>
  <strong>Default: 300.0</strong></li>

  <li><strong>FallBack_Speed</strong><br>
  Speed of BOTs while moving backward.<br>
  <strong>Default: 220.0</strong></li>

  <li><strong>Bot_ForwardTo_Distance</strong><br>
  Enable BOTs to move toward distant targets.<br>
  1 = Enabled, 0 = Disabled<br>
  <strong>Default: 1</strong></li>

  <li><strong>Forward_Distance</strong><br>
  BOTs will move toward targets if farther than this.<br>
  <strong>Default: 400.0</strong></li>

  <li><strong>Forward_Speed</strong><br>
  Speed of BOTs while moving forward.<br>
  <strong>Default: 220.0</strong></li>

  <li><strong>Block_BotUse_Grenade</strong><br>
  Prevent BOTs from using grenades (CS:S bug fix).<br>
  1 = Block, 0 = Allow<br>
  <strong>Default: 1</strong></li>
</ul>
