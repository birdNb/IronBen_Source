//摄像头
//拍照并保存
//打开摄像头
function getMedia() {
    let constraints = {
        //参数
        video: {width: 1270, height: 870},
        audio: true
    };
    //获得video摄像头区域
    let video = document.getElementById("video");
    //返回的Promise对象
    let promise = navigator.mediaDevices.getUserMedia(constraints);
    //then()异步，调用MediaStream对象作为参数
    promise.then(function (MediaStream) {
        video.srcObject = MediaStream;
        video.play();
    });
}
//拍照
function takePhoto() {

    let canvas = document.getElementById("canvas");
    let ctx = canvas.getContext('2d');
    //绘图
    ctx.drawImage(video, 0, 0,200,100);
}
//清除拍摄的相片
function clearPhoto() {
    let canvas = document.getElementById("canvas");
    let ctx = canvas.getContext('2d');
    ctx.clearRect(0,0,200,100); 
}
//关闭摄像头，并清除拍摄的相片
function stopMedia(videoElem) {
    clearPhoto();
    const stream = videoElem.srcObject; 
    const tracks = stream.getTracks();
    tracks.forEach(function(track) {track.stop();});
    videoElem.srcObject = null;
} 
//下载拍摄下来的图片
function downloadImg()
{
    var url = canvas.toDataURL({format: "image/png", quality:2, width:1270, height:870});
    var a = document.createElement('a');
    var event = new MouseEvent('click');
    a.download = 'photo';
    a.href = url;
    a.dispatchEvent(event);
}