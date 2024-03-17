import math
last=0


def addition( a):
    global last
    k= a[0]+a[1] if len(a)==2 else a[0]+last
    last=k
    return k


def soustraction( a):
    global last
    k= a[0]-a[1] if len(a)==2 else last - a[0]
    last=k
    return k

def multiplication(a):
    global last
    k= a[0]*a[1] if len(a)==2 else a[0]*last
    last=k
    return k



def division(a):
    global last
    k= a[0]/a[1] if len(a)==2 else last/a[0]
    last=k
    return k

def modulo(a):
    global last
    k= a[0]%a[1] if len(a)==2 else last%a[0]
    last=k
    return k

def sin(a):
    return math.sin(a[0])


def cos(a):
    return math.cos(a[0])


def tan(a):
    return math.tan(a[0])