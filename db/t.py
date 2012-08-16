import sqlite3
con = sqlite3.connect('i-imgur-com')
cur = con.cursor()

aid = "a123"
cur.execute('select id from image where article_id=?',(aid,))
print cur.fetchone()
