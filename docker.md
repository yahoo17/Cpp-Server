
Linux - MySQL-Docker 安装配置
1. 拉取mysql镜像#

Copy
# 使用网易的 MySQL 镜像地址
docker pull hub.c.163.com/library/mysql:5.7

# 为镜像重命名
docker tag hub.c.163.com/library/mysql:5.7 mysql:57

2. 创建用于挂载的目录#

Copy
# 用于挂载mysql数据文件
sudo mkdir /my/mysql/datadir

# 用于挂载mysql配置文件
sudo mkdir /my/mysql/my.cnf

# 修改/my目录拥有者
sudo chown user:docker /my

3. 使用镜像创建容器#

Copy
# 启动容器
## 命名为 mysql57
## 分别做了端口映射33060
## 挂载在之前创建的文件夹上
## 密码初始化为123456
docker run -dit --name mysql57 -p 33060:3306 
        -v /my/mysql/datadir:/var/lib/mysql 
        -v /my/mysql/my.cnf:/etc/my.cnf 
        -e MYSQL_ROOT_HOST='%'
        -e MYSQL_ROOT_PASSWORD='123456' mysql:57

# 命令解析
## --name: 容器名
## -p: 映射宿主主机端口
## -v: 挂载宿主目录到容器目录
## -e: 设置环境变量，此处指定root密码
## -d: 后台运行容器

4. 使用 MySQL#

Copy
# 可以先进入容器, 然后在容器中访问 mysql
docker exec -it mysql57 bash
mysql -uroot -p123456

# 也可以从宿主机连接
mysql -uroot -P 33061 -h127.0.0.1 -p123456

MySQL 官方Docker镜像的使用
