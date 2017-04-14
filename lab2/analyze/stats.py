
import matplotlib.pyplot as plt
import numpy as np

plt.style.use('ggplot')

def analyze():
    allTimes=[]
    files=["1/GlobalLock/LockFree/", "1/ReadWriteLock/LockFree/"]

    for filepath in files:
    	fd=open(filepath+"stats.csv", "r")
    
    	times=[]
    	get=[]
    	post=[]
    	delete=[]
    	unknown=[]

    	percentsReqs=[]
    	percentsTime=[]
    	request_times=[0,0,0,0]
    	request_amounts=[0,0,0,0]
    	totalReqs=0

    	for line in fd:
        	req=line.split(",")
        	reqType=req[0]
        	reqTime=req[1]
        	times.append(reqTime)
        	reqTime=float(reqTime)
        	totalReqs+=1

        	if(reqType=="GET"):
            		request_amounts[0]+=1
            		request_times[0]+=reqTime
            		get.append(reqTime)
        	elif(reqType=="POST"):
            		request_amounts[1]+=1
            		request_times[1]+=reqTime
            		post.append(reqTime)
        	elif(reqType=="DELETE"):
            		request_amounts[2]+=1
            		request_times[2]+=reqTime
            		delete.append(reqTime)
        	else:
            		request_amounts[3]+=1
            		request_times[3]+=reqTime
            		unknown.append(reqTime)

    	for i in range(len(request_times)):
        	amount=request_amounts[i]
        	percentsReqs.append((amount/totalReqs)*100)
        	if(not amount):
            		amount=1
        	percentsTime.append((request_times[i]/amount)*100)

    	amountGraph(filepath+'amounts.jpg', request_amounts)
    	piTypeGraph(percentsReqs, filepath+'percentages_Requests.jpg')
    	piTimeGraph(percentsTime, filepath+'percentages_Times.jpg')
    	graph(times, filepath+'graph.jpg', 'all requests')
    	graph(get, filepath+'getGraph.jpg', 'get requests')
    	graph(post, filepath+'postGraph.jpg', 'post requests')
    	graph(delete, filepath+'deleteGraph.jpg', 'delete requests')
    	graphType(filepath+'typesGraph.jpg', get, post, delete, unknown)

    	mx=max(times)
    	mn=min(times)
    
    	times.sort()
    	size=len(times)
    	if(size%2==0):
        	mdn=(float(times[int(size/2)-1])+float(times[int(size/2)]))/2
    	else:
        	mdn=times[int(size/2)]

    	avg=average(times)

    	print("Maximum Time: "+str(mx)+"\nMinimum Time: "+str(mn)+"\nMedian: "+str(mdn)+"\nAverage: "+str(avg))

	allTimes.append(times)

    count=-1
    for times in allTimes:
    	line, =plt.plot(times, linewidth=2.0, label=files[++count]);
    	plt.legend(handles=[line])
    
    display('All Runs', 'Time (seconds)', 'Requests')

def average(arr):
    avg=0.0
    tot=len(arr)

    for elem in arr:
        avg+=float(elem)

    return avg/tot

def piTypeGraph(percents, name):
    labels = 'Gets', 'Posts', 'Deletes', 'Unknowns'
    explode = (0.1, 0, 0, 0)

    fig1, ax1 = plt.subplots()
    ax1.pie(percents, explode=explode, labels=labels, autopct='%1.1f%%',
            shadow=True, startangle=90)
    ax1.axis('equal')
    plt.savefig(name)

def piTimeGraph(percents, name):
    labels = 'Gets', 'Posts', 'Deletes', 'Unknowns'
    explode = (0.1, 0, 0, 0)

    fig1, ax1 = plt.subplots()
    ax1.pie(percents, explode=explode, labels=labels, autopct='%1.1f%%',
            shadow=True, startangle=90)
    ax1.axis('equal')
    plt.savefig(name)

def graph(times, name, label):
    line, =plt.plot(times, linewidth=2.0, label=label);
    plt.legend(handles=[line])
    display(name, 'Time (seconds)', 'Requests')

def amountGraph(name, amounts):
    objects = ('Gets', 'Posts', 'Deletes', 'Unknowns')
    y = np.arange(len(objects))
    
    plt.bar(y, amounts, align='center', alpha=0.5);
    plt.xticks(y, objects)

    display(name, 'Types', 'Requests')

def graphType(name, gets, posts, deletes, unknowns):
    getLine, =plt.plot(gets, linewidth=2.0, label="gets")
    postLine, =plt.plot(posts, linewidth=2.0, label="posts")
    deleteLine, =plt.plot(deletes, linewidth=2.0, label="deletes")
    unknownLine, =plt.plot(unknowns, linewidth=2.0, label="unknowns")
    
    plt.legend(handles=[getLine, postLine, deleteLine, unknownLine])
    display(name, 'Requests', 'Time (seconds)')

def display(save, x, y):
    plt.grid(True)
    plt.ylabel(x)
    plt.xlabel(y)
    plt.title(save)
    plt.savefig(save)

if __name__ == "__main__":
    analyze()