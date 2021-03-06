function connectOnce(sig, slot) {
    var f = function() {
        slot.apply(this, arguments)
        sig.disconnect(f)
    }
    sig.connect(f)
}

function readableFileSize(bytes, si) {
    var thresh = si ? 1000 : 1024;
    if(Math.abs(bytes) < thresh) {
        return bytes + ' B';
    }
    var units = si
        ? ['kB','MB','GB','TB','PB','EB','ZB','YB']
        : ['KiB','MiB','GiB','TiB','PiB','EiB','ZiB','YiB'];
    var u = -1;
    do {
        bytes /= thresh;
        ++u;
    } while(Math.abs(bytes) >= thresh && u < units.length - 1);
    return bytes.toFixed(1)+' '+units[u];
}
function readFile(fileUrl) {
    var request = new XMLHttpRequest();
    request.open("GET", fileUrl, false);
    request.send(null);
    return request.responseText;
}
function urlToPath(urlString) {
    var s
    if (urlString.startsWith("file:///")) {
        var k = urlString.charAt(9) === ':' ? 8 : 7
        s = urlString.substring(k)
    } else {
        s = urlString
    }
    return decodeURIComponent(s);
}
function qmlTypeOf(object, className){
   let objectString = object.toString();
   return objectString.startsWith(className)
}
function isAccessible(object){
    return typeof(object)!=="undefined" && object !== null
}
