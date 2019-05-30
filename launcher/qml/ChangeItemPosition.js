function next() {
    repeater.currentItem += 1;
    if (repeater.currentItem > repeater.count - 1)
    {
        repeater.currentItem = 0;
    }
    var count = 0;

    for (var i = 0; i < repeater.count; i++)
    {
        var item = repeater.itemAt(i);
        item.currentState -= 1;
        if ( item.currentState < 1 )
        {
            item.currentState = repeater.count;
        }
        if ( item.currentState >= 1 && item.currentState <= 10 )
        {
            item.state = "pos" + item.currentState
        } else if ( item.currentState === 15 ){
            item.state = "pos11";
        } else if ( item.currentState === 16 ){
            item.state = "pos12";
        } else if ( item.currentState === 17 ){
            item.state = "pos13";
        } else if ( item.currentState >= 18 ){
            item.state = "pos0";
        } else {
            item.state = "pos10";
        }
    }
}

function prev() {
    repeater.currentItem -= 1;
    if (repeater.currentItem < 0)
    {
        repeater.currentItem = repeater.count - 1;
    }
    var count = repeater.count;
    for (var i = 0; i < repeater.count; i++)
    {
        var item = repeater.itemAt(i);
        item.currentState += 1;
        if ( item.currentState > repeater.count )
        {
            item.currentState = 1;
        }
        if ( item.currentState >= 1 && item.currentState <= 10 )
        {
            item.state = "pos" + item.currentState
        } else if ( item.currentState === 15 ){
            item.state = "pos11";
        } else if ( item.currentState === 16 ){
            item.state = "pos12";
        } else if ( item.currentState === 17 ){
            item.state = "pos13";
        } else if ( item.currentState >= 18 ){
            item.state = "pos0";
        } else {
            item.state = "pos10";
        }
    }
}

function move(pos, item) {
    var distance = firstPox - pos;
    if(distance > 200) {
        next()
        firstPox = pos
        iconMoved = true
        item.iconReleased()
    } else if (distance < -200) {
        prev()
        firstPox = pos
        iconMoved = true
        item.iconReleased()
    }
}

function getCenterItem() {
    var item;
    for (var i = 0; i < repeater.count; i++)
    {
        item = repeater.itemAt(i)
        if (item.state === "pos5" || item.state === "pos14")
            return item
    }
}
