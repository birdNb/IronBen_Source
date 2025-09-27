//命令
//小车的运动模块，只做演示，因为具体的实现需要后端的配合
//设置
    let Img1 = document.getElementById("img1");//姿态一
    Img1.style.border = "5px solid blue";
    let Img2 = document.getElementById("img2");//姿态二
    Img2.style.border = "5px solid blue";
    let Img3 = document.getElementById("img3");//姿态三
    Img3.style.border = "5px solid blue";
    let Img4 = document.getElementById("img4");//姿态四
    Img4.style.border = "5px solid blue";    
    let Img5 = document.getElementById("img5");//上行
    Img5.style.border = "5px solid blue";    
    let Img6 = document.getElementById("img6");//下行
    Img6.style.border = "5px solid blue";    
    let Img7 = document.getElementById("img7");//启用云台
    Img7.style.border = "5px solid blue";    
    let Img8 = document.getElementById("img8");//越野步态
    Img8.style.border = "5px solid blue";
function changeBorder() {
    let Img1 = document.getElementById("img1");//姿态一
    Img1.style.border = "5px solid blue";
    let Img2 = document.getElementById("img2");//姿态二
    Img2.style.border = "5px solid blue";
    let Img3 = document.getElementById("img3");//姿态三
    Img3.style.border = "5px solid blue";
    let Img4 = document.getElementById("img4");//姿态四
    Img4.style.border = "5px solid blue";
    let Img5 = document.getElementById("img5");//上行
    Img5.style.border = "5px solid blue";
    let Img6 = document.getElementById("img6");//下行
    Img6.style.border = "5px solid blue";
    let Img7 = document.getElementById("img7");//启用云台
    Img7.style.border = "5px solid blue";
    let Img8 = document.getElementById("img8");//越野步态
    Img8.style.border = "5px solid blue";
}

function carCommand1() {
        changeBorder();
        let img = document.getElementById("img1");//姿态一
        img.style.border = "5px solid red";    
}

function carCommand2() {
        changeBorder();
        let img = document.getElementById("img2");//姿态二
        img.style.border = "5px solid red";
}

function carCommand3() {
        changeBorder();
        let img = document.getElementById("img3");//姿态三
        img.style.border = "5px solid red";    
}

function carCommand4() {
        changeBorder();
        let img = document.getElementById("img4");//姿态四
        img.style.border = "5px solid red";    
}

function carCommand5() {
        changeBorder();
        let img = document.getElementById("img5");//上行
        img.style.border = "5px solid red";    
}

function carCommand6() {
        changeBorder();
        let img = document.getElementById("img6");//下行
        img.style.border = "5px solid red";    
}

function carCommand7() {
        changeBorder();
        let img = document.getElementById("img7");//启用云台
        img.style.border = "5px solid red";    
}

function carCommand8() {
        changeBorder();
        let img = document.getElementById("img8");//启用越野姿态
        img.style.border = "5px solid red";    
}

document.addEventListener("keydown",carRun);
function carRun(event)
{
    //控制小车的运动
    if(event.keyCode === 87) {alert("W");}
    else if(event.keyCode === 83) {alert("S");}
    else if(event.keyCode === 65) {alert("A");}
    else if(event.keyCode === 68) {alert("D");}
    //控制小车的姿态
    if(event.keyCode === 49) { carCommand1();}
    else if(event.keyCode === 50) {carCommand2();}
    else if(event.keyCode === 51) {carCommand3();}
    else if(event.keyCode === 52) {carCommand4();}
    else if(event.keyCode === 81) {carCommand5();}
    else if(event.keyCode === 69) {carCommand6();}
    else if(event.keyCode === 70) {carCommand7();}
    else if(event.keyCode === 84) {carCommand8();}       
    //拍照功能的快捷键
    if(event.keyCode === 85) {getMedia();}//开启摄像头(u)
    else if(event.keyCode === 73) {stopMedia(video);}//关闭摄像头(i)
    else if(event.keyCode === 74) {takePhoto();}//拍照(j)
    else if(event.keyCode === 72) {clearPhoto();}//删除照片(h)
    else if(event.keyCode === 75) {downloadImg();}//下载图片(k)
}


