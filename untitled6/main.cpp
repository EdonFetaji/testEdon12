#include <iostream>
#include <cstring>

using namespace std;

class ExistingPodcast {
public:
    void message() {
        cout << "The podcast is already in the collection" << endl;
    }
};

class Podcast {
protected:
    char name[100];
    char url[100];
    float cost;
    int subDiscount;
public:
    Podcast(char *name = (char *) "", char *url = (char *) "", float cost = 0, int subDiscount = 0) :
            cost(cost), subDiscount(subDiscount) {
        strcpy(this->name, name);
        strcpy(this->url, url);
    }

    virtual float price() {
        float total = cost;
        total *= (1.0 - (subDiscount / 100.));
        return total;
    }

    friend ostream &operator<<(ostream &os, Podcast &podcast);

    friend istream &operator>>(istream &i, Podcast &podcast);

    bool operator==(const Podcast &other) {
        return !strcmp(this->url, other.url);
    }

    friend class Playlist;
};

class StreamingPodcast : public Podcast {
private:
    float monthlyFee;
    int month, year;
public:
    StreamingPodcast(char *name = (char *) "", char *url = (char *) "", float cost = 0, int subDiscount = 0,
                     float monthlyFee = 0, int month = 0, int year = 0) :
            Podcast(name, url, cost, subDiscount), monthlyFee(monthlyFee), month(month), year(year) {}

    friend ostream &operator<<(ostream &os, Podcast &podcast);

    friend istream &operator>>(istream &i, Podcast &podcast);

    friend class Playlist;

    float price() override {
        float p = Podcast::price();
        int months;
        if (year < 2018) {
            months = (12 - this->month) + (2017 - year) * 12 + 5;
        } else {
            months = 5 - this->month;
        }

        p += months * monthlyFee;
        return p;
    }
};

ostream &operator<<(ostream &os, Podcast &podcast) {
    os << "Podcast: " << podcast.name << "\nURL: " << podcast.url << "\nregular price: $" << podcast.cost;
    if (podcast.subDiscount) {
        os << ", bought on discount";
    }
    StreamingPodcast *casted = dynamic_cast<StreamingPodcast *>(&podcast);
    if (casted) {
        os << ", monthly fee: $" << casted->monthlyFee << ", purchased: " << casted->month << "-" << casted->year;
    }
    return os;
}

istream &operator>>(istream &i, Podcast &podcast) {
    i.get();
    i.getline(podcast.name, 100);
    i.getline(podcast.url, 100);
    i >> podcast.cost >> podcast.subDiscount;
    StreamingPodcast *casted = dynamic_cast<StreamingPodcast *>(&podcast);
    if (casted) {
        i >> casted->monthlyFee >> casted->month >> casted->year;
    }

    return i;
}

class Playlist {
private:
    char playlistName[100];
    Podcast **podcasts;
    int n;
public:
    Playlist(char *playlistName) {
        strcpy(this->playlistName, playlistName);
        podcasts = new Podcast *[0];
        n = 0;
    }

    Playlist &operator+=(Podcast &newPodcast) {
        for (int i = 0; i < n; ++i) {
            if (!strcmp(podcasts[i]->name, newPodcast.name)) {
                throw ExistingPodcast();
            }
        }
        Podcast **tmp = new Podcast *[n + 1];
        for (int i = 0; i < n; ++i) {
            tmp[i] = podcasts[i];
        }
        delete[]podcasts;
        tmp[n++] = &newPodcast;
        podcasts = tmp;
        return *this;
    }

    float total_spent() {
        float total = 0;
        for (int i = 0; i < n; ++i) {
            total += podcasts[i]->price();
        }
        return total;
    }

    friend ostream &operator<<(ostream &os, const Playlist &playlist) {
        os << "Playlist: " << playlist.playlistName << endl;
        for (int i = 0; i < playlist.n; ++i) {
            StreamingPodcast *casted = dynamic_cast<StreamingPodcast *>(playlist.podcasts[i]);
            if (casted) {
                os << *casted << endl;
            } else {
                os << *playlist.podcasts[i] << endl;
            }

        }
        return os;
    }
};


int main() {
    int test_case_num;

    cin >> test_case_num;

    // for Podcast
    char podcast_name[100];
    char podcast_url[1000];
    float podcast_price;
    int podcast_discount;

    // for StreamingPodcast
    float sub_podcast_monthly_fee;
    int sub_podcast_month, sub_podcast_year;

    // for Playlist
    char name[100];
    int num_podcasts_in_playlist;

    if (test_case_num == 1) {
        cout << "Testing class Podcast and operator&lt;&lt; for Podcast" << std::endl;
        cin.get();
        cin.getline(podcast_name, 100);
        cin.getline(podcast_url, 1000);
        //cin.get();
        cin >> podcast_price >> podcast_discount;

        Podcast p(podcast_name, podcast_url, podcast_price, podcast_discount);

        cout << p;
    } else if (test_case_num == 2) {
        cout << "Testing class StreamingPodcast and operator&lt;&lt; for StreamingPodcast" << std::endl;
        cin.get();
        cin.getline(podcast_name, 100);
        cin.getline(podcast_url, 1000);
        //cin.get();
        cin >> podcast_price >> podcast_discount;

        cin >> sub_podcast_monthly_fee;
        cin >> sub_podcast_month >> sub_podcast_year;

        StreamingPodcast sp(podcast_name, podcast_url, podcast_price, podcast_discount, sub_podcast_monthly_fee,
                            sub_podcast_month, sub_podcast_year);
        cout << sp;
    } else if (test_case_num == 3) {
        cout << "Testing operator&gt;&gt; for Podcast" << std::endl;
        Podcast p;

        cin >> p;

        cout << p;
    } else if (test_case_num == 4) {
        cout << "Testing operator&gt;&gt; for StreamingPodcast" << std::endl;
        StreamingPodcast sp;

        cin >> sp;

        cout << sp;
    } else if (test_case_num == 5) {
        cout << "Testing class Playlist and operator+= for Playlist" << std::endl;
        cin.get();
        cin.getline(name, 100);
        Playlist u(name);

        int num_user_podcasts;
        int podcast_type;
        cin >> num_user_podcasts;

        for (int i = 0; i < num_user_podcasts; ++i) {

            cin >> podcast_type;

            Podcast *g;
            // 1 - Game, 2 - SubscriptionGame
            if (podcast_type == 1) {
                cin.get();
                cin.getline(podcast_name, 100);
                cin.getline(podcast_url, 1000);

                cin >> podcast_price >> podcast_discount;
                g = new Podcast(podcast_name, podcast_url, podcast_price, podcast_discount);
            } else if (podcast_type == 2) {
                cin.get();
                cin.getline(podcast_name, 100);
                cin.getline(podcast_url, 1000);

                cin >> podcast_price >> podcast_discount;

                cin >> sub_podcast_monthly_fee;
                cin >> sub_podcast_month >> sub_podcast_year;
                g = new StreamingPodcast(podcast_name, podcast_url, podcast_price, podcast_discount,
                                         sub_podcast_monthly_fee, sub_podcast_month, sub_podcast_year);
            }

            //cout<<(*g);
            try {
                u += (*g);
            } catch (ExistingPodcast &e) {
                e.message();
            }


        }
        cout << endl;
        cout << u;

    } else if (test_case_num == 6) {
        cout << "Testing exception ExistingPodcast for Playlist" << std::endl;
        cin.get();
        cin.getline(name, 100);
        Playlist u(name);

        int num_podcasts_in_playlist;
        int podcast_type;
        cin >> num_podcasts_in_playlist;

        for (int i = 0; i < num_podcasts_in_playlist; ++i) {

            cin >> podcast_type;

            Podcast *g;
            // 1 - Game, 2 - SubscriptionGame
            if (podcast_type == 1) {
                cin.get();
                cin.getline(podcast_name, 100);
                cin.getline(podcast_url, 1000);

                cin >> podcast_price >> podcast_discount;
                g = new Podcast(podcast_name, podcast_url, podcast_price, podcast_discount);

            } else if (podcast_type == 2) {
                cin.get();
                cin.getline(podcast_name, 100);
                cin.getline(podcast_url, 1000);

                cin >> podcast_price >> podcast_discount;

                cin >> sub_podcast_monthly_fee;
                cin >> sub_podcast_month >> sub_podcast_year;
                g = new StreamingPodcast(podcast_name, podcast_url, podcast_price, podcast_discount,
                                         sub_podcast_monthly_fee, sub_podcast_month, sub_podcast_year);
            }

            try {
                u += (*g);
            } catch (ExistingPodcast &e) {
                e.message();
            }

        }
        cout << endl;
        cout << u;
    } else if (test_case_num == 7) {
        cout << "Testing total_spent method() for Playlist" << std::endl;
        cin.get();
        cin.getline(name, 100);
        Playlist u(name);

        int num_podcasts_in_playlist;
        int podcast_type;
        cin >> num_podcasts_in_playlist;

        for (int i = 0; i < num_podcasts_in_playlist; ++i) {

            cin >> podcast_type;

            Podcast *g;
            // 1 - Game, 2 - SubscriptionGame
            if (podcast_type == 1) {
                cin.get();
                cin.getline(podcast_name, 100);
                cin.getline(podcast_url, 1000);

                cin >> podcast_price >> podcast_discount;
                g = new Podcast(podcast_name, podcast_url, podcast_price, podcast_discount);
            } else if (podcast_type == 2) {
                cin.get();
                cin.getline(podcast_name, 100);
                cin.getline(podcast_url, 1000);

                cin >> podcast_price >> podcast_discount;

                cin >> sub_podcast_monthly_fee;
                cin >> sub_podcast_month >> sub_podcast_year;
                g = new StreamingPodcast(podcast_name, podcast_url, podcast_price, podcast_discount,
                                         sub_podcast_monthly_fee, sub_podcast_month, sub_podcast_year);
            }

            //cout<<(*g);
            try {
                u += (*g);
            } catch (ExistingPodcast &e) {
                e.message();
            }

        }

        cout << endl;
        cout << u;
        cout << endl;
        cout << "Total money spent: $" << u.total_spent() << endl;
    }
}