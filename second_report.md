# <center>数字媒体处理综合实验</center>
<center>
<br><br><br><br><br><br><br><br><br><br>
<font size = 4>
姓名： 桑燊   

班级： 2014211602  

学号： 2014212128  

<br><br><br><br><br><br><br><br><br>
时间： 2017年03月10日
</font size = 4>
<br><br><br><br><br><br><br><br>
</center>

## 实验目标
完成实验一的后续功能：

* 图片位置和大小自适应
* 同一目录中的图片切换

## 实验思路
首先是图片位置和大小自适应，思路如下：  

* 使用`QGraphicsPixmap::setPos()，setScale()`设置图片的位置和大小 
* 使用`QGraphicsScene::setSceneRect()`进行QGraphicsScene大小适配
* 使用`QGraphicsView::fitInView()`进行GraphicsView的大小适配 

在最初文件选择的基础功能上，设置同一目录的图片切换，设计思路如下：

* 根据选择记录当前文件的上级路径
* 通过文件后缀匹配找出并且记录该路径下的所有图片路径
* 先找出当前显示的图片的序列号并且记录下来（使用全局变量）
* 每次点击上一张或者下一张的时候，根据当前的序列号查找上一张或者下一张并且进行显示

## 实验步骤

1. 在`MyMouseEvent.cpp`中构造一个`reSet()`函数，该函数用来初始化和复位图片的位置和大小，代码如下：
		
		void MyMouseEvent::reSet()
		{
		    setPos(0, 0);
		    setScale(1.0);
		}
		
2. 在`MainWindow.cpp`中构造一个`reSet()`函数，该函数用来重新设置当前`QGraphicsView，QGraphicsScene，MyMouseEvent`对象的布局及大小关系，代码如下：

		void MainWindow::reSet()
		{
		    // 若当前没有图片显示
		    if(NULL == gpi){
		        return;
		    }
		
		    // 调用gpi的初始化函数
		    gpi->reSet();
		
		    // QGraphics大小自适应
		    gs->setSceneRect(gpi->boundingRect());
		    // graphicsView的自适应
		    ui->graphicsView->fitInView(gpi->boundingRect(),
		    	Qt::KeepAspectRatio);
		}
	
3. 在**复位**按钮的槽函数中调用该`reSet()`函数即可，同时将其添加到**打开文件**槽函数中，这样每次打开图片都是自适应显示的。
4. 在获取到图片以后，需要获取其绝对路径信息，使用`FileDialog::directory()`获取上级路径信息得到一个QDir对象，通过该对象的`QDir::entryInfoList(Filters filters = NoFilter)`函数可以获取该路径下所有符合filters的文件，将filters定义为符合图片后缀的字符串数组即可。

		void MainWindow::on_btn_open_clicked(){
		
		    if(fileDialog->exec()){
		        files = fileDialog->selectedFiles();
		
		        QDir dir = fileDialog->directory();
		        
		        QStringList filters;
		        filters << "*.jpg" << "*.bmp" << "*.png";
		        // 获取文件
		        images = dir.entryInfoList(filters);
		
		        for(int i = 0; i < images.length(); i++){
		            if(files[0] == images[i].absoluteFilePath()){
		                curIndex = i;
		            }
		        }
		        showImage(QImage(files[0]));
		    }
		}
	
5. 给按钮**上一张**和**下一张**添加槽函数，代码如下：

		// 上一张
		void MainWindow::on_btn_last_clicked()
		{
		    // 移除当前对象
		    gs->removeItem(gpi);
		    gpi = new MyMouseEvent();
		    if(images.length() == 0){
		        return;
		    }
		
		    showImage(QImage(images[(images.length() + --curIndex)
		    % images.length()].absoluteFilePath()));
		}
		
	**下一张**槽函数代码类似。

## 实验结果
* 初始界面：

<center>
<img src="/Users/sangshen/Desktop/HelloSangShen/大学作业/大三下学期/数字媒体综合处理/第二次作业/sourceImages/e.png" width="50%"/>  
图 1.  初始界面
</center>

* 打开一张图片并且将其缩小：

<center>
<img src="/Users/sangshen/Desktop/HelloSangShen/大学作业/大三下学期/数字媒体综合处理/第二次作业/sourceImages/b.png" width="50%"/>  
图 2.  缩小图片
</center>

* 重置图片：

<center>
<img src="/Users/sangshen/Desktop/HelloSangShen/大学作业/大三下学期/数字媒体综合处理/第二次作业/sourceImages/c.png" width="50%"/>  
图 3.  重置图片
</center>

* 切换图片：

点击**上一张**
<center>
<img src="/Users/sangshen/Desktop/HelloSangShen/大学作业/大三下学期/数字媒体综合处理/第二次作业/sourceImages/a.png" width="50%"/>  
图 4.  上一张图片
</center>


点击*两次* **下一张**按钮  
<center>
<img src="/Users/sangshen/Desktop/HelloSangShen/大学作业/大三下学期/数字媒体综合处理/第二次作业/sourceImages/d.png" width="50%"/>  
图 5.  下一张图片
</center>
