import urllib2
import re
import sqlite3

con = sqlite3.connect('../db/i-imgur-com')
cur = con.cursor()
ent = 'http://www.click-me.com.tw/category/Funny.html'
html = urllib2.urlopen(ent).read()
re_articl = r'article/(\d+)\.html'
re_img    = r'src="http:\/\/i\.imgur\.com\/(\w+)\.jpg"'
articles  = re.findall(re_articl, html)

article   = [articles[i] for i in range(len(articles)) if i%2 == 0]
for a in article:
	cur.execute('select id from image where article_id = ?', (a,))
	if(cur.fetchone()):
		print 'parsed this article before!'
		continue
	url = 'http://www.click-me.com.tw/article/'+a+'.html'
	print url
	det = urllib2.urlopen(url).read()
	imgs= re.findall(re_img, det)
	if(imgs):
		for img in imgs:
			src = 'http://i.imgur.com/'+img+'.jpg';
			saveImg = open('../imgs/'+a+'-'+img+'.jpg', 'wb')
			saveImg.write(urllib2.urlopen(src).read())
			saveImg.close()
			try:
				cur.execute('insert into image (article_id, img_key) values (?, ?)', (a, img))
				con.commit()
			except:
				pass
			print "image %s-%s.jpg saved!" % (a, img)
	else:
		print 'no image found!'
cur.close()
