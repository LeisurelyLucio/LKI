# LKI
应用：Lucio King Illusion

版本：1.3

=====================================================================================================

上传了几张图片作为测试用例，自己选择图片时，注意最好用24位无损格式图片！否则可能会出现重复刷新、选择错误等问题

=====================================================================================================

操作与功能说明：

由于未进行多线程处理，注意在进行长时间处理操作时不要进行其他任何操作(尤其是点击)，否则会因线程占用被系统判定为恶意软件而死机，在右下进度条完成后方可进行其他操作！

1>	打开、保存图像：1>点击界面右上角图标->打开、保存菜单；2>标题栏左部快捷栏按钮 ！！注意另存为时只能覆盖现有文件 ！！ ！！请尽量使用24位位深度格式文件，对于其他格式由于未进行适配，可能会出现选择功能选择集错误问题 ！！ 另外打开的图像会展示在界面左半边，对于有损压缩图片，显示保存时可能会有黑线。本工程中的自适应是锁定宽高比的。

2>	设置颜色值C1和C2：功能区->选择菜单->颜色设置面板->设置颜色C1、C2按钮 ！！注意通过弹出对话框显示的RGB值查看是否已选中你想选中的颜色 ！！颜色值C1代表选择集的显示颜色，需要在选择之前设置；C2可以用于填充选择集

3>	界面右半边展示RGB三维分析：功能区->主页菜单->色彩分析面板->RGB分析按钮 ！！ 注意能够处理的图像大小最大为4000*2000 ！！！！预处理功能较慢，请耐心等待 ！！ 界面右半边会生成以RGB值为三维坐标的笛卡尔坐标系，并将打开图片中所有的像素点对应绘制在坐标系中

4>	放大、缩小及坐标系旋转：功能区->主页菜单->缩放及旋转面板->各个按钮 由于坐标系采用投影到RG平面的斜二测投影方式，因此坐标系旋转看起来像是仅改动了一个坐标轴的位置；在图片大小改动不满意时，可以通过还原默认值按钮自适应显示

5>	图片及坐标系平移：功能区->主页菜单->鼠标模式面板->选中移动工具，之后可以通过鼠标拖拽进行图片或坐标系的移动 可以通过主页->平移面板还原初始位置

6>	展示图像原图或者只展示当前的选择集：功能区->选择菜单->图像显示面板->复选按钮  ！！仅显示选择集功能较慢，请耐心等待 ！！选择集使用用颜色C1进行展示

7>	选择模式：功能区->主页菜单->鼠标模式面板->选中选择工具，之后在选择面板中选择工具类型以及选择方式 ！！ 选择工具在图像或者坐标系中均可使用 ！！！！当选择区域较大时，请耐心等待 ！！
图像像素选择集S的初始状态为空。对选择集允许有并、交、差的三种状态可以供给选择。设当前拾取的像素集合为T，则
“并”意味着: S=S+T，即在T中的像素也都加入到选择集S中。
“交”意味着: S=S∩T，即选择集S只包含同时在S与T中的像素。
“差”意味着: S=S+T，即从选择集S中踢除在T中的像素。
在界面左半边，允许(1) 拾取单个像素; (2) 拾取一个矩形内的像素; (3)拾取一个圆形内的像素。拾取结果即为T，最终选择集S的结果由上面并、交、差运算而得。
在界面右半边，允许(1) 拾取单个颜色值; (2) 拾取一个矩形(体)内的颜色值; (3)拾取一个圆形(或球体)内的颜色值。拾取结果T为颜色值与选中的颜色值相等的所有像素，最终选择集S的结果由上面并、交、差运算而得。

8>	取消当前选择集并查看处理效果：功能区->选择菜单->图像显示面板->取消选择

9>	填充颜色：功能区->选择菜单->颜色设置面板->填充按钮  将当前选择集S内所有像素的颜色值替换为颜色值C2

10>	简单滤镜： 功能区->滤镜菜单->各种滤镜按钮 将当前图像选择集应用滤镜效果，并取消选择集；若未指定选择集，则对整个图像应用效果



