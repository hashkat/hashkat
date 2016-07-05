if (!Detector.webgl) {
        Detector.addGetWebGLMessage();
    } else {

        var years = ['1', '2', '3'];
        var container = document.getElementById('container');
        var globe = new DAT.Globe(container);
        console.log(globe);
        var i, tweens = [];

        var settime = function(globe, t) {
            return function() {
                new TWEEN.Tween(globe).to({
                    time: t / years.length
                }, 500).easing(TWEEN.Easing.Cubic.EaseOut).start();
                var y = document.getElementById('option' + years[t]);
                if (y.getAttribute('class') === 'option active') {
                    return;
                }
                var yy = document.getElementsByClassName('option');
                for (i = 0; i < yy.length; i++) {
                    yy[i].setAttribute('class', 'option');
                }
                y.setAttribute('class', 'option active');
            };
        };

        for (var i = 0; i < years.length; i++) {
            var y = document.getElementById('option' + years[i]);
            y.addEventListener('mouseover', settime(globe, i), false);
        }

        var xhr;
        TWEEN.start();


        xhr = new XMLHttpRequest();
        xhr.open('GET', './json/newtest.json', true);
        xhr.onreadystatechange = function(e) {
            if (xhr.readyState === 4) {
                if (xhr.status === 200) {
                    var data = JSON.parse(xhr.responseText);
                    window.data = data;
                    for (i = 0; i < data.length; i++) {
                        globe.addData(data[i][1], {
                            format: 'magnitude',
                            name: data[i][0],
                            animated: true
                        });
                    }
                    globe.createPoints();
                    settime(globe, 0)();
                    globe.animate();
                }
            }
        };
        xhr.send(null);
    }
