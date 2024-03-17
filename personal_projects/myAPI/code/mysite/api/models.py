from django.db import models

# Create your models here.

class BlogPost(models.Model):
    title=models.CharField(max_length=50)
    content= models.TextField()
    date= models.DateField(auto_now_add=True)
    
    def __str__(self) :
        return self.title
    

class Emotion(models.Model):
    nom_emotion = models.CharField(max_length=50)

    def __str__(self):
        return self.nom_emotion

class Book(models.Model):
    title = models.CharField(max_length=255)
    author = models.CharField(max_length=255)
    description = models.TextField()
    genre = models.CharField(max_length=100)
    publication_date = models.DateField(auto_now_add=True)
    emotions = models.ManyToManyField(Emotion)

    def __str__(self):
        return self.title
    
    
    


class Review(models.Model):
    book = models.ForeignKey(Book, on_delete=models.CASCADE)
    rating = models.IntegerField()
    comment = models.TextField()
    review_date = models.DateField(auto_now_add=True)

    def __str__(self):
        return f"Review of {self.book.title} - {self.rating}"
